//!*******************************************************
//! TsPrimitiveMeshCreater.h
//!
//! refrence.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

struct TsVertexSkin;
struct TsVertexDefault;
struct TSVertexSkyBox;

class TsPrimitiveMeshCreater
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------

    static TsBool GenerateSphere( TsVertexSkin** ppOutVertex, 
                                  TsInt& outVertexCount,
                                  TsInt div = 10);
    static TsBool GenerateSphere( TsVertexDefault** ppOutVertex, 
                                  TsInt& outVertexCount,
                                  TsInt div = 10);

    static TsBool GenerateCapsule3D( TsVertexSkin** ppOutVertex,
                                     TsInt& outVertexCount,
                                     TsInt slices,TsInt stack);

    static TsBool GenerateSprite( TsVertexSkin** ppOutVertex,
                                  TsInt& outVertexCount,
                                  TS_CULL_MODE cullMode = TS_CULL_MODE::BACK);
    static TsBool GenerateSprite( TsVertexDefault** ppOutVertex,
                                  TsInt& outVertexCount,
                                  TS_CULL_MODE cullMode = TS_CULL_MODE::BACK);

    static TsBool GenerateSpriteLine( TsVertexSkin** ppOutVertex ,
                                  TsInt& outVertexCount);
    static TsBool GenerateSpriteLine( TsVertexDefault** ppOutVertex ,
                                  TsInt& outVertexCount  );

    static TsBool GenerateBox3D ( TsVertexSkin** ppOutVertex ,
                                  TsInt& outVertexCount);
    static TsBool GenerateBox3D ( TsVertexDefault** ppOutVertex ,
                                  TsInt& outVertexCount);

    static TsBool GenerateBox3DLine( TsVertexSkin** ppOutVertex ,
                                 TsInt& outVertexCount );
    static TsBool GenerateBox3DLine( TsVertexDefault** ppOutVertex ,
                                 TsInt& outVertexCount );

    static TsBool GenrateInvBox3D( TsVertexSkin** ppOutVertex ,
                                   TsInt& outVertexCount);
    static TsBool GenrateInvBox3D( TsVertexDefault** ppOutVertex,
                                   TsInt& outVertexCount);
    static TsBool GenrateInvBox3D( TSVertexSkyBox** ppOutVertex,
                                   TsInt& outVertexCount);

};