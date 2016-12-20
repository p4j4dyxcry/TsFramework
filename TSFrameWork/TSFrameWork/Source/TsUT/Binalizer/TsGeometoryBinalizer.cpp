#include "../../../TsAfx.h"
#include "TsBinalizerBase.h"
#include "TsMaterialBinalizer.h"
#include "TsTransformBinalizer.h"
#include "TsSkeletonBinalizer.h"
#include "TsGeometoryBinalizer.h"
#include <fstream>
TsBool TsGeometoryBinalizer::Binalize(TsDevice* pDev,
                                      std::ofstream& ofs,
                                      TsGeometryObject* pData)
{
    TsVertexElement* pElement = pData->GetVertexElement();

    TsVertexBuffer* pVertexBuffer = pElement->GetVertexBuffer();
    TsIndexBuffer*  pIndexBuffer = pElement->GetIndexBuffer();
    TsTransForm*    pTransform = pData->GetTransform();
    TsMaterial*     pMaterial = pData->GetMaterial();


    TsUint vertexBufferSize = 0; 
    TsUint indexBufferSize = 0;

    TsVertexSkin * pVertexArray = nullptr;
    TsUint       * pIndexArray = nullptr;

    auto GetNativeBuffer = [](TsDevice* pDev, 
                              TsBuffer* pBuffer)
    {
        void * temp;
        TsUint sz = pBuffer->GetBufferSize();
        void* pResult = TsNew(TsByte[sz]);

        temp = pDev->GetDC()->Map(pBuffer, D3D11_MAP_READ);
        memcpy(pResult, temp, sz);
        pDev->GetDC()->UnMap(pBuffer);

        return pResult;
    };

    if (pVertexBuffer)
    {
        vertexBufferSize = pVertexBuffer->GetBufferSize();
        pVertexArray = (TsVertexSkin*)GetNativeBuffer(pDev, pVertexBuffer);
    }
    if (pIndexBuffer)
    {
        indexBufferSize = pIndexBuffer->GetBufferSize();
        pIndexArray = (TsUint*)GetNativeBuffer(pDev, pIndexBuffer);
    }

    if (WriteHeader(ofs, typeid(*this).name()) == TS_FALSE)
        return TS_FALSE;

    ofs.write((TsChar*)vertexBufferSize, sizeof(TsUint));
    ofs.write((TsChar*)indexBufferSize, sizeof(TsUint));

    if (vertexBufferSize )
        ofs.write((TsChar*)pVertexArray, vertexBufferSize);
    if ( indexBufferSize)
        ofs.write((TsChar*)pIndexArray, indexBufferSize);
    CommonRef ref;
    memset(&ref, 0, sizeof(ref));

    strcpy_s(ref.name, pData->GetName().c_str());
    strcpy_s(ref.matName, pMaterial->GetName().c_str());
    ref.transformPtr = (TsU64)pTransform;    

    ofs.write((TsChar*)&ref, sizeof(ref));
    TsSafeDelete(pVertexArray);
    TsSafeDelete(pIndexArray);

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


    TsUint vertexSize = 0;
    TsUint indexSize = 0;

    ifs.read((TsChar*)vertexSize, sizeof(TsUint));
    ifs.read((TsChar*)indexSize, sizeof(TsUint));

    TsVertexSkin* pVertex = nullptr;
    TsUint *      pIndex = nullptr;

    TsVertexElement* pElement = TsNew(TsVertexElement);
    if (vertexSize)
    {
        ifs.read((TsChar*)pVertex, vertexSize);
        pVertex = TsNew(  TsVertexSkin[vertexSize / sizeof(TsVertexSkin)] );
        auto pVB =pDev->CreateVertexBuffer(pVertex, vertexSize, sizeof(TsVertexSkin));
        pElement->SetVertexBuffer(pVB);
    }
    if (indexSize)
    {
        ifs.read((TsChar*)pIndex, indexSize);
        pIndex = TsNew(TsUint[indexSize / sizeof(TsUint)]);
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
        TsTransForm* pTransform = pTransformBinalizer->FindByBinalyPtr(ref.transformPtr);
        p->SetTransform(pTransform);
        p->SetSkeleton(pSkeletonBinalizer->GetSkeleton());

        m_pGeometoryObject = p;
    }    
    else
    {
        m_pGeometoryObject = TsNew(TsSkinGeometryObject);
        TsTransForm* pTransform = pTransformBinalizer->FindByBinalyPtr(ref.transformPtr);
        m_pGeometoryObject->SetTransform(pTransform);
    }

    return TS_TRUE;
}