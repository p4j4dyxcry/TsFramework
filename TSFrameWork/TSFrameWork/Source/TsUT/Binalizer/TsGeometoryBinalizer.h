//!*******************************************************
//! TsGeometoryBinalizer.h
//!
//! ジオメトリバイナライザ.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsGeometoryBinalizer : public TsBinalizer
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsGeometoryBinalizer();
    struct CommonRef
    {
        TsChar name[128];
        TsChar matName[128];
        TsU64  transformPtr;
    };

    TsBool Binalize(TsDevice* pDev,
                    std::ofstream& ofs, 
                    TsGeometryObject* pData);
    TsBool Decode(TsDevice* pDev,
                  std::ifstream& ifs,
                  TsTransformBinalizer* pTransformBinalizer,
                  TsMaterialBinalizer * pMaterialBinalizer,
                  TsSkeletonBinalizer * pSkeletonBinalizer,
                  TsBool readHeader = TS_FALSE);

    TsGeometryObject* GetGeometry()const
    {
        return m_pGeometoryObject;
    }
    
protected:
    //----------------------------------------------------------
    // peropery
    //----------------------------------------------------------
    TsGeometryObject* m_pGeometoryObject;
};