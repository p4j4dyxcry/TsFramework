#include "../../../TsAfx.h"
#include "TsBinalizerBase.h"
#include "TsMaterialBinalizer.h"
#include "TsTransformBinalizer.h"
#include "TsSkeletonBinalizer.h"
#include "TsGeometoryBinalizer.h"
#include <fstream>

TsGeometoryBinalizer::TsGeometoryBinalizer()
    :m_pGeometoryObject(nullptr)
    , m_geometoryCount(0)
{

}

TsGeometoryBinalizer::~TsGeometoryBinalizer()
{
    for each (auto g in m_pGeometoryRef)
    {
        TsSafeDelete(g.m_pVertexArray);
        TsSafeDelete(g.m_pIndexArray);
    }
}



TsBool TsGeometoryBinalizer::Binalize(std::ofstream& ofs,
                                      TsGeometryObject** ppGeometory,
                                      TsUint count)
{
    if (WriteHeader(ofs, TS_BIN_HEADER) == TS_FALSE)
        return TS_FALSE;
    ofs.write((TsChar*)&count, sizeof(TsUint));

    for (TsUint i = 0; i < count; ++i)
    {
        const TsGeometryObject* pData = ppGeometory[i];

        TsVertexElement* pElement = pData->GetVertexElement();

        TsVertexBuffer* pVertexBuffer = pElement->GetVertexBuffer();
        TsIndexBuffer*  pIndexBuffer = pElement->GetIndexBuffer();
        TsTransForm*    pTransform = pData->GetTransform();
        TsMaterial*     pMaterial = pData->GetMaterial();


        TsUint vertexBufferSize = 0;
        TsUint indexBufferSize = 0;

        TsVertexSkin * pVertexArray = nullptr;
        TsUint       * pIndexArray = nullptr;

        if (pVertexBuffer)
        {
            vertexBufferSize = pVertexBuffer->GetBufferSize();
            pVertexArray = (TsVertexSkin*)pVertexBuffer->GetOriginData();
        }
        if (pIndexBuffer)
        {
            indexBufferSize = pIndexBuffer->GetBufferSize();
            pIndexArray = (TsUint*)pIndexBuffer->GetOriginData();
        }

        ofs.write((TsChar*)&vertexBufferSize, sizeof(TsUint));
        ofs.write((TsChar*)&indexBufferSize, sizeof(TsUint));

        if (vertexBufferSize)
            ofs.write((TsChar*)pVertexArray, vertexBufferSize);
        if (indexBufferSize)
            ofs.write((TsChar*)pIndexArray, indexBufferSize);
        CommonRef ref;
        memset(&ref, 0, sizeof(ref));

        strcpy_s(ref.name, pData->GetName().c_str());
        strcpy_s(ref.matName, pMaterial->GetName().c_str());
        ref.transformPtr = (TsU64)pTransform;
        ref.aabb = pData->GetAABB();
        ofs.write((TsChar*)&ref, sizeof(ref));
    }


    return TS_TRUE;

}
TsBool TsGeometoryBinalizer::Decode( std::ifstream& ifs )
{
    ifs.read((TsChar*)&m_geometoryCount, sizeof(TsUint));

    m_pGeometoryObject = TsNew(TsGeometryObject*[m_geometoryCount]);

    m_pGeometoryRef.resize(m_geometoryCount);
    m_pCommonRef.resize(m_geometoryCount);
    for (TsUint geoIndex = 0; geoIndex < m_geometoryCount; ++geoIndex)
    {
        TsUint vertexSize = 0;
        TsUint indexSize = 0;

        ifs.read((TsChar*)&vertexSize, sizeof(TsUint));
        ifs.read((TsChar*)&indexSize, sizeof(TsUint));

        m_pGeometoryRef[geoIndex].m_vertexCount = vertexSize / sizeof(TsVertexSkin);
        m_pGeometoryRef[geoIndex].m_indexCount = indexSize / sizeof(TsUint);

        if (vertexSize)
        {
            m_pGeometoryRef[geoIndex].m_pVertexArray = TsNew(TsVertexSkin[vertexSize / sizeof(TsVertexSkin)]);
            ifs.read((TsChar*)m_pGeometoryRef[geoIndex].m_pVertexArray, vertexSize);
        }
        if (indexSize)
        {
            m_pGeometoryRef[geoIndex].m_pIndexArray = TsNew(TsUint[indexSize / sizeof(TsUint)]);
            ifs.read((TsChar*)m_pGeometoryRef[geoIndex].m_pIndexArray, indexSize);
        }
        ifs.read((TsChar*)&m_pCommonRef[geoIndex], sizeof(CommonRef));
    }

    return TS_TRUE;
}

//! 頂点バッファ・インデックスバッファを含むメッシュ作成
TsBool TsGeometoryBinalizer::BuildGeometory(TsDevice * pDev)
{
    for (TsInt i = 0; i < m_geometoryCount; ++i)
    {
        TsVertexElement* pElement = TsNew(TsVertexElement);

        TsVertexSkin* pVertex = m_pGeometoryRef[i].m_pVertexArray;
        TsInt         vertexSize = m_pGeometoryRef[i].m_vertexCount * sizeof(TsVertexSkin);
        TsUint*       pIndex = m_pGeometoryRef[i].m_pIndexArray;
        TsInt         indexSize = m_pGeometoryRef[i].m_indexCount * sizeof(TsUint);


        if (vertexSize > 0)
        {
            TsVertexBuffer* pVB = pDev->CreateVertexBuffer(pVertex, vertexSize, sizeof(TsVertexSkin));
            pElement->SetVertexBuffer(pVB);
        }

        if (indexSize > 0)
        {

            TsIndexBuffer* pIB = pDev->CreateIndexBuffer(pIndex, indexSize);
            pElement->SetIndexBuffer(pIB);
        }

        if (vertexSize && pVertex[0].weight.x > 0)
        {
            TsSkinGeometryObject* p = TsNew(TsSkinGeometryObject);
            p->CreateGeometryObject(pDev, pElement);
            m_pGeometoryObject[i] = p;
        }
        else if ( vertexSize )
        {
            m_pGeometoryObject[i] = TsNew(TsGeometryObject);
            m_pGeometoryObject[i]->CreateGeometryObject(pDev, pElement);
        }
        else
        {
            return TS_FALSE;
        }

        m_pGeometoryObject[i]->SetAABB(m_pCommonRef[i].aabb);
        m_pGeometoryObject[i]->SetName(m_pCommonRef[i].name);
    }

    return TS_TRUE;
}

//! トランスフォームをバインドする
TsBool TsGeometoryBinalizer::BindTransform(TsTransformBinalizer* pTransformBinalizer)
{
    for (TsInt i = 0; i < m_geometoryCount; ++i)
    {
        TsTransForm* pTransform = pTransformBinalizer->FindByBinalyPtr(m_pCommonRef[i].transformPtr);
        m_pGeometoryObject[i]->SetTransform(pTransform);
    }

    return TS_TRUE;
}

//! マテリアルをバインドする
TsBool TsGeometoryBinalizer::BindMaterial(TsMaterialBinalizer * pMaterialBinalizer)
{
    TsDefaultMaterial* pMatArray = pMaterialBinalizer->GetMaterials();
    TsDefaultMaterial * m = nullptr;
    TsUint matCount = pMaterialBinalizer->GetMaterialCount();

    for (TsUint geometoryIdx = 0; geometoryIdx < m_geometoryCount; ++geometoryIdx)
    {
        for (TsUint i = 0; i < matCount; ++i)
        {
            if (pMatArray[i].GetName() == m_pCommonRef[i].matName)
            {
                m_pGeometoryObject[geometoryIdx]->SetMaterial(&pMatArray[i]);
                break;
            }
        }
    }


    return TS_TRUE;
}

//! スケルトンをバインドする
TsBool TsGeometoryBinalizer::BindSkeleton(TsSkeletonBinalizer* pSkeletonBinalizer)
{
    
    for (TsUint geometoryIdx = 0; geometoryIdx < m_geometoryCount; ++geometoryIdx)
    {
        TsVertexSkin* pOrigin = (TsVertexSkin*)m_pGeometoryObject[geometoryIdx]->GetVertexElement()->GetVertexBuffer()->GetOriginData();
        if (pOrigin->weight.x > 0)
        {
            TsSkinGeometryObject* p = dynamic_cast<TsSkinGeometryObject*>(m_pGeometoryObject[geometoryIdx]);
            p->SetSkeleton(pSkeletonBinalizer->GetSkeleton());
        }
    }

    return TS_TRUE;
}