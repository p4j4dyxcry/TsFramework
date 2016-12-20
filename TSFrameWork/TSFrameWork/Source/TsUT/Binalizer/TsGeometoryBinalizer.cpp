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


TsBool TsGeometoryBinalizer::Binalize(TsDevice* pDev,
                                      std::ofstream& ofs,
                                      const TsGeometryObject* pGeometory,
                                      TsUint count)
{
    if (WriteHeader(ofs, typeid(*this).name()) == TS_FALSE)
        return TS_FALSE;
    ofs.write((TsChar*)&count, sizeof(TsUint));

    for (TsUint i = 0; i < count; ++i)
    {
        const TsGeometryObject* pData = &pGeometory[i];

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
        TsSafeDelete(pVertexArray);
        TsSafeDelete(pIndexArray);
    }


    return TS_TRUE;

}
TsBool TsGeometoryBinalizer::Decode(TsDevice* pDev,
                  std::ifstream& ifs,
                  TsTransformBinalizer* pTransformBinalizer,
                  TsMaterialBinalizer * pMaterialBinalizer,
                  TsSkeletonBinalizer * pSkeletonBinalizer,
                  TsBool readHeader )
{
    if (readHeader)
    {
        if (ReadHeader(ifs, typeid(*this).name()) == TS_FALSE)
            return TS_FALSE;
    }

    ifs.read((TsChar*)&m_geometoryCount, sizeof(TsUint));

    m_pGeometoryObject = TsNew(TsGeometryObject*[m_geometoryCount]);

    for (TsUint geoIndex = 0; geoIndex < m_geometoryCount; ++geoIndex)
    {
        TsUint vertexSize = 0;
        TsUint indexSize = 0;

        ifs.read((TsChar*)&vertexSize, sizeof(TsUint));
        ifs.read((TsChar*)&indexSize, sizeof(TsUint));

        TsVertexSkin* pVertex = nullptr;
        TsUint *      pIndex = nullptr;

        TsVertexElement* pElement = TsNew(TsVertexElement);
        if (vertexSize)
        {
            pVertex = TsNew(TsVertexSkin[vertexSize / sizeof(TsVertexSkin)]);
            ifs.read((TsChar*)pVertex, vertexSize);
            auto pVB = pDev->CreateVertexBuffer(pVertex, vertexSize, sizeof(TsVertexSkin));
            pElement->SetVertexBuffer(pVB);
        }
        if (indexSize)
        {
            pIndex = TsNew(TsUint[indexSize / sizeof(TsUint)]);
            ifs.read((TsChar*)pIndex, indexSize);
            auto pIB = pDev->CreateIndexBuffer(pIndex, indexSize);
            pElement->SetIndexBuffer(pIB);
        }

        CommonRef ref;
        ifs.read((TsChar*)&ref, sizeof(ref));

        TsDefaultMaterial* pMatArray = pMaterialBinalizer->GetMaterials();
        TsDefaultMaterial * m = nullptr;
        TsUint matCount = pMaterialBinalizer->GetMaterialCount();

        for (TsUint i = 0; i < matCount; ++i)
        {
            if (pMatArray[i].GetName() == ref.matName)
            {
                m = &pMatArray[i];
                break;
            }
        }


        if (pVertex[0].weight.x > 0)
        {
            auto p = TsNew(TsSkinGeometryObject);
            p->CreateGeometryObject(pDev, pElement, m);
            p->SetSkeleton(pSkeletonBinalizer->GetSkeleton());
            m_pGeometoryObject[geoIndex] = p;
        }
        else
        {
            m_pGeometoryObject[geoIndex] = TsNew(TsGeometryObject);
            m_pGeometoryObject[geoIndex]->CreateGeometryObject(pDev, pElement, m);
        }
        TsTransForm* pTransform = pTransformBinalizer->FindByBinalyPtr( ref.transformPtr );
        m_pGeometoryObject[geoIndex]->SetTransform( pTransform );
        m_pGeometoryObject[geoIndex]->SetAABB( ref.aabb );
    }

    return TS_TRUE;
}