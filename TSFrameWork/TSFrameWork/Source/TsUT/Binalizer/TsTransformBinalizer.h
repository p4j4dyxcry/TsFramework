//!*******************************************************
//! TsTransformBinalizer.h
//!
//! Transformをバイナリ化するクラス.
//!
//! © 2016 Yuki Tsuneyama
#pragma once


class TsTransformBinalizer : public TsBinalizer
{
public:
    //----------------------------------------------------------
    // Define
    //----------------------------------------------------------

#pragma pack(1)
    struct TransformCommon
    {
        TsChar          name[128];
        TsVector3       pos;
        TsQuaternion    q;
        TsVector3       scale;
        TsU64           parentPtr;
        TsU64           thisPtr;
    };
#pragma pack()


    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsTransformBinalizer();
    
    TsBool Binalize(std::ofstream& ofs,TsTransForm* pData);
    TsBool Decode( std::ifstream& ifs );

    TsTransForm* GetRootTransform()const
    {
        return m_pRootTransform;
    }

    TsTransForm* FindByBinalyPtr(TsU64 data)
    {
        return m_transformMap[data];
    }

protected:
    //----------------------------------------------------------
    // peropery
    //----------------------------------------------------------
    TsMap<TsU64, TsTransForm*> m_transformMap;
    TsTransForm* m_pRootTransform;
};