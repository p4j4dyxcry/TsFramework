
#include "../../../TsAfx.h"

TsMeshObject* Ts3DMeshConverter::ConvertFromFile(TsDevice* pDev,
                                                 const TsChar* filename)
{

    TSUT::TsFilePathAnalyzer analizer(filename);


    TsMap<void*, TsMaterial*> pMateriaMap;

    struct Hash
    {
        std::size_t operator()( const TsVertexSkin& key ) const
        {
            return key.pos.x * 2.1245f +
                key.pos.y * 4.215f +
                key.pos.z * 3.1415f +
                key.uv.x  * 50.25f +
                key.uv.y  * 2048.5f +
                key.normal.x * 32.5f +
                key.normal.y * 12.2f +
                key.weight.x * 15.5f +
                key.weight.y * 62.5f +
                key.weight.z * 85.1f +
                key.weight.w * 55.0f +
                key.boneIndex.x * 5 +
                key.boneIndex.y * 25 +
                key.boneIndex.z * 125 +
                key.boneIndex.w * 565;
        }
    };

    TsVector<TsVertexSkin> VertexList;
    TsMap<TsVertexSkin , TsUint , Hash> hashMap;
    TsVector<TsUint>       indexList;

    auto pLoader = ExtencionToModelLoader( analizer.GetExtension().c_str() );

    if (pLoader != nullptr)
    {
        TsBool hr = pLoader->LoadFile(filename);
        pLoader->CreateCommonData();

        if (hr == TS_FALSE)
        {
            return TS_FALSE;
        }

        TsCommon3DMesh*      commonMesh = pLoader->GetMeshData();
        TsCommon3DMaterial*  commonMaterial = pLoader->GetMaterialData();
        TsSkeleton*          pSkeleton = pLoader->GetSkeletonData();
        TsTransForm*         pRootTransform = pLoader->GetRootTransformData();

        TsUint meshCount = pLoader->GetMeshCount();
        TsUint materialCount = pLoader->GetMaterialCount();
      
        TsMeshObject* pMesh = TsNew( TsMeshObject );

        for (TsUint i = 0; i < materialCount; ++i)
        {
            TsMaterial* pMat = ConvertMaterial(pDev, filename, &commonMaterial[i]);
            pMateriaMap.insert(
                std::pair<void*, TsMaterial*>(commonMaterial, pMat));
        }


        for (TsUint i = 0; i < meshCount; ++i)
        {
            VertexList.resize(0);
            indexList.resize(0);
            auto & m = commonMesh[i];
            if (m.m_pIndex)
            {
                for (TsUint j = 0; j < m.m_indexCount; ++j)
                {
                    TsUint idx = 0;
                    if (m.m_pIndex)
                        idx = m.m_pIndex[j];

                    TsVertexSkin&& vertex = ConvertVertx(&m, idx);

                    auto it = hashMap.find( vertex );

                    if( it == hashMap.end() )
                    {
                        TsUint index = VertexList.size();
                        hashMap.insert( std::pair<TsVertexSkin , TsUint>( vertex , index ) );
                        indexList.push_back(VertexList.size());
                        VertexList.push_back(vertex);
                    }
                    else
                    {
                        indexList.push_back(it->second);
                    }
                }
            }
            else
            {
                for (TsUint j = 0; j < m.m_posCount; ++j)
                {
                    TsUint idx = j;

                    TsVertexSkin&& vertex = ConvertVertx(&m, idx);

                    auto it = tstl::find(VertexList.begin(), VertexList.end(), vertex);

                    if (it == VertexList.end())
                    {
                        indexList.push_back(VertexList.size());
                        VertexList.push_back(vertex);
                    }
                    else
                    {
                        indexList.push_back(tstl::distance(VertexList.begin(), it));
                    }
                }
            }

            TsVertexBuffer* vb = pDev->CreateVertexBuffer(
                &VertexList[0],
                sizeof(TsVertexSkin) * VertexList.size(),
                sizeof(TsVertexSkin));

            TsIndexBuffer* ib = pDev->CreateIndexBuffer(
                &indexList[0],
                sizeof(TsUint) * indexList.size());

            TsVertexElement * vertexElement = TsNew(TsVertexElement);
            vertexElement->m_vertexBuffer = vb;
            vertexElement->m_indexBuffer = ib;

            TsGeometryObject* pGeometory = nullptr;
            if (m.m_weightCount > 0)
            {
                TsSkinGeometryObject * skin =
                    TsNew(TsSkinGeometryObject);
                skin->CreateGeometryObject(pDev, vertexElement, pMateriaMap[m.m_pMaterialRef]);
                skin->SetSkeleton(pSkeleton);
                pGeometory = skin;
            }
            else
            {
                pGeometory = TsNew(TsGeometryObject);
                pGeometory->CreateGeometryObject(pDev, vertexElement, pMateriaMap[m.m_pMaterialRef]);
            }
            pGeometory->SetName(m.m_name);
            pGeometory->SetTransform(m.m_pTransoform);
            pMesh->AddGeometry(pGeometory);
        }
        TsSafeDelete(pLoader);
        return pMesh;
    }

    return nullptr;
   
}

Ts3DModelLoader* Ts3DMeshConverter::ExtencionToModelLoader(const TsChar * extencion)
{
    auto ext = TSUT::StringToLower(extencion);

    if (ext == ".stl") return TsNew ( TsStlLoader ) ;

    return nullptr;

}

TsMaterial *  Ts3DMeshConverter::ConvertMaterial(TsDevice* pDev,
                                                 const TsChar * filename, 
                                                 TsCommon3DMaterial* pCommonMat)
{
    TsDefaultMaterial* pMat = TsNew(TsDefaultMaterial);

    pMat->CreateMaterial(pDev);

    TSUT::TsFilePathAnalyzer ana = filename;

    if (pCommonMat->m_albedoTexture.size())
    {
        TsString filepath = ana.GetLocalDirectory() + pCommonMat->m_albedoTexture;
        pMat->LoadAlbedoTextureFromFile(pDev,filepath);
    }

    if (pCommonMat->m_specluerTexture.size())
    {
        TsString filepath = ana.GetLocalDirectory() + pCommonMat->m_specluerTexture;
        pMat->LoadSpeculerTextureFromFile(pDev, filepath);
    }

    if (pCommonMat->m_normalTexture.size())
    {
        TsString filepath = ana.GetLocalDirectory() + pCommonMat->m_normalTexture;
        pMat->LoadNormalTextureFromFile(pDev, filepath);
    }

    pMat->SetName(pCommonMat->m_name);
    TsFloat4 diffuse(pCommonMat->m_diffuse.x,
                     pCommonMat->m_diffuse.y,
                     pCommonMat->m_diffuse.z,
                     pCommonMat->m_alpha);
    pMat->SetColor(diffuse);
    TsFloat4 ambient(pCommonMat->m_ambient.x,
                     pCommonMat->m_ambient.y,
                     pCommonMat->m_ambient.z,
                     0);
    pMat->SetAmbient(ambient);
    pMat->SetSpeculer(pCommonMat->m_specluer);
    pMat->SetSpeculerPower(pCommonMat->m_power);

    return pMat;
}


TsVertexSkin Ts3DMeshConverter::ConvertVertx(TsCommon3DMesh* pMesh, TsUint index)
{
    TsVertexSkin v;
    if (pMesh->m_pPositions)
        v.pos = pMesh->m_pPositions[index];

    if (pMesh->m_pNormals)
        v.normal = pMesh->m_pNormals[index];

    if (pMesh->m_pTexcoords)
        v.uv = pMesh->m_pTexcoords[index];
       
    if (pMesh->m_pWeights)
        v.weight = pMesh->m_pWeights[index];

    if (pMesh->m_pBoneIndex)
        v.boneIndex = pMesh->m_pBoneIndex[index];

    return v;
}