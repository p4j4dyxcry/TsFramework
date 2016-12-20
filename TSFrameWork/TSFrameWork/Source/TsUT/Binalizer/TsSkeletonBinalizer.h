﻿//!*******************************************************
//! TsSkeletonBinalizer.h
//!
//! スケルトンのバイナライズ.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsSkeletonBinalizer : public TsBinalizer
{
public:
#pragma pack(1)
   
    struct CommonBone
    {
        TsChar      name[128];
        TsInt       id;
        TsMatrix    bindPoseMatrix;
        TsU64       transformPtr;
    };

#pragma pack()
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsSkeletonBinalizer();

    TsBool Binalize(std::ofstream& ofs, TsSkeleton* pData);
    TsBool Decode(std::ifstream& ifs, 
                  TsTransformBinalizer* pTransformBinalizer,
                  TsBool readHeader = TS_FALSE);

    TsSkeleton* GetSkeleton()const
    {
        return m_pSkeleton;
    }
protected:    
    //----------------------------------------------------------
    // peropery
    //----------------------------------------------------------
    TsSkeleton* m_pSkeleton;
};