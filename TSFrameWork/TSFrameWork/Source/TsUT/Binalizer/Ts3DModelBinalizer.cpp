#include "../../../TsAfx.h"
#include "../../TsGfx/TsGfx.h"
#include "TsBinalizerBase.h"
#include "TsMaterialBinalizer.h"
#include "TsTransformBinalizer.h"
#include "TsSkeletonBinalizer.h"
#include "TsGeometoryBinalizer.h"
#include "Ts3DModelBinalizer.h"

#include <fstream>

Ts3DModelBinalizer::Ts3DModelBinalizer()
    :m_pMeshObjects(nullptr)
    , m_meshCount(0)
{

}

TsBool Ts3DModelBinalizer::LoadBinaly(TsDevice* pDev,const TsChar* filename)
{    
    std::ifstream ifs(filename, std::ios::binary);

    if (ReadHeader(ifs, TS_BIN_HEADER) == TS_FALSE)
        return TS_FALSE;

    ifs.read((TsChar*)&m_meshCount, sizeof(TsUint));

    m_pMeshObjects = TsNew(TsMeshObject[m_meshCount]);

    for (TsUint i = 0; i < m_meshCount; ++i)
    {
        TsTransformBinalizer transformBinalizer;
        TsMaterialBinalizer  materialBinalizer;
        TsSkeletonBinalizer  skeletonBinalizer;
        TsGeometoryBinalizer geometoryBinalizer;
       
        while (!ifs.eof())
        {
            TsBinalizer::BinalizerHeader header;
            memset(&header, 0, sizeof(header));
            ifs.read((TsChar*)&header, HEADER_SIZE);

            if ( header == transformBinalizer.GetBinaryHeader() )
                transformBinalizer.Decode(ifs);

            if (header == materialBinalizer.GetBinaryHeader())
            {
                materialBinalizer.Decode(ifs);
                materialBinalizer.BuildMaterial(pDev);
            }

            if (header == skeletonBinalizer.GetBinaryHeader())
            {
                skeletonBinalizer.Decode(ifs);
                skeletonBinalizer.BuildSkeleton(&transformBinalizer);
                m_pMeshObjects[i].SetSkeleton(skeletonBinalizer.GetSkeleton());
            }

            if (header == geometoryBinalizer.GetBinaryHeader())
            {
                geometoryBinalizer.Decode(ifs);
                geometoryBinalizer.BuildGeometory(pDev);
                geometoryBinalizer.BindMaterial(&materialBinalizer);
                geometoryBinalizer.BindTransform(&transformBinalizer);
                geometoryBinalizer.BindSkeleton(&skeletonBinalizer);

                TsGeometryObject** pGeo = geometoryBinalizer.GetGeometry();
                for (TsUint j = 0; j < geometoryBinalizer.GetGeometoryCount(); ++j)
                    m_pMeshObjects[i].AddGeometry(pGeo[j]);
            }
        }

    }
    TsDebugLogLoadSuccess(filename);
    return TS_TRUE;

}

TsBool Ts3DModelBinalizer::SaveBinaly(TsDevice* pDev,
                                      const TsChar* filename,
                                      TsMeshObject* pMeshObjects,
                                      TsUint meshCount)
{
    TsDebugLog("Save Model Cache -> \"file = %s\" \n", filename);
    std::ofstream ofs(filename, std::ios::binary);

    if (WriteHeader(ofs, TS_BIN_HEADER) == TS_FALSE)
        return TS_FALSE;

    TsTransformBinalizer transformBinalizer;
    TsMaterialBinalizer  materialBinalizer;
    TsSkeletonBinalizer  skeletonBinalizer;
    TsGeometoryBinalizer geometoryBinalizer;

    ofs.write((TsChar*)&meshCount, sizeof(TsUint));

    for (TsUint i = 0; i < meshCount; ++i)
    {
        TsMeshObject& m = pMeshObjects[i];
        transformBinalizer.Binalize(ofs, m.GetRootTransform());

        auto&& matList = m.GetMaterials();
        auto&& geoList = m.GetGeometrys();
        materialBinalizer.Binalize(ofs, &matList[0],matList.size());
        skeletonBinalizer.Binalize(ofs, m.GetSkeleton());
        geometoryBinalizer.Binalize(ofs, &geoList[0], m.GetGeometryCount());
    }
    TsDebugLogSuccess("Save Success -> \"file = %s\" \n", filename);
    return TS_TRUE;
}
