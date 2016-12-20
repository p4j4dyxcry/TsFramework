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

    if (ReadHeader(ifs, typeid(*this).name()) == TS_FALSE)
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
            TsChar header[HEADER_SIZE]="";
            ifs.read(header, HEADER_SIZE);
            if (strcmp(header, typeid(TsTransformBinalizer).name()) == 0)
                transformBinalizer.Decode(ifs);

            if (strcmp(header, typeid(TsMaterialBinalizer).name()) == 0)
                materialBinalizer.Decode(pDev, ifs);

            if (strcmp(header, typeid(TsSkeletonBinalizer).name()) == 0)
            {
                skeletonBinalizer.Decode(ifs, &transformBinalizer);
                m_pMeshObjects[i].SetSkeleton(skeletonBinalizer.GetSkeleton());
            }

            if (strcmp(header, typeid(TsGeometoryBinalizer).name()) == 0)
            {
                geometoryBinalizer.Decode(pDev,
                    ifs,
                    &transformBinalizer,
                    &materialBinalizer,
                    &skeletonBinalizer);

                TsGeometryObject** pGeo = geometoryBinalizer.GetGeometry();
                for (TsUint j = 0; j < geometoryBinalizer.GetGeometoryCount(); ++j)
                    m_pMeshObjects[i].AddGeometry(pGeo[j]);

            }
        }

    }

    return TS_TRUE;

}

TsBool Ts3DModelBinalizer::SaveBinaly(TsDevice* pDev,
                                      const TsChar* filename,
                                      TsMeshObject* pMeshObjects,
                                      TsUint meshCount)
{
    std::ofstream ofs(filename, std::ios::binary);

    if (WriteHeader(ofs, typeid(*this).name()) == TS_FALSE)
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
        materialBinalizer.Binalize(ofs, &matList[0],matList.size());
        skeletonBinalizer.Binalize(ofs, m.GetSkeleton());
        geometoryBinalizer.Binalize(pDev, ofs, m.GetGeometry(0),m.GetGeometryCount());
    }
    return TS_TRUE;
}
