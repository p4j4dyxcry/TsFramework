//!*******************************************************
//! TsTransformBinalizer.h
//!
//! Transformをバイナリ化するクラス.
//!
//! © 2016 Yuki Tsuneyama
#pragma once


class TsTransformBinalizer
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

    TsBool       WriteTransform(std::ofstream& ofs,TsTransForm* pRoot);
    TsTransForm* ReadTransform( std::ifstream& ifs,TsBool readHeader = TS_FALSE );

protected:
    //----------------------------------------------------------
    // peropery
    //----------------------------------------------------------
    
};