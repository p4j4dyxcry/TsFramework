#pragma once
#include<fbxsdk.h>
#ifdef _DEBUG 
#pragma comment(lib,"debug/libfbxsdk-md.lib")
#else
#pragma comment(lib,"release/libfbxsdk-md.lib")
#endif

#define TsFbxNodeAttributeType FbxNodeAttribute::EType

template<typename FbxType>
void FbxSafeRelease( FbxType* pFbx )
{
    if( pFbx )
        pFbx->Destroy();
    pFbx = nullptr;
}
static TsMatrix FbxMatrixToTsMatrix( const FbxMatrix& mtx )
{
    TsMatrix m;

    for( int i = 0; i < 16; ++i )
        m.m[0][i] = static_cast<float>( mtx[0][i] );

    return m;
}


static const TsInt TS_FBX_MAX_UV = ( 6 );

// fbx vertex
struct TsFbxVertex
{
    TsFloat3 pos;
    TsFloat3 normal;	//
    TsFloat4 boneWeight;
    TsInt4   boneIndex;
    TsFloat3 tangent;
    TsFloat3 binormal;
    TsColor  color;
    TsFloat2 uv[TS_FBX_MAX_UV];		//uv x 8 layer

    TsBool operator == ( const TsFbxVertex& v )
    {
        return
            pos == v.pos &&
            normal == v.normal&&
            tangent == v.tangent &&
            binormal == v.binormal&&
            uv[0] == v.uv[0] &&
            uv[1] == v.uv[1] &&
            uv[2] == v.uv[2] &&
            uv[3] == v.uv[3] &&
            uv[4] == v.uv[4] &&
            uv[5] == v.uv[5];
    }
};

struct TsFbxDefaultVertex
{
    TsFloat3 pos;
    TsFloat3 normal;
    TsFloat2 uv0;
};

struct TsFbxDefaultSkinVertex
{
    TsFloat3 pos;
    TsFloat3 normal;
    TsFloat2 uv;
    TsFloat4 weight;
    TsInt4   boneIdx;
};

// fbx face
struct TsFbxFace
{
    union
    {
        struct
        {
            TsInt posIndex[3];
            TsInt normalIndex[3];
            TsInt tangentIndex[3];
            TsInt binormalIndex[3];
            TsInt colorIndex[3];
            TsInt UVIndex[3][TS_FBX_MAX_UV];

            TsInt finalIndex[3];
        };
        TsInt data[38];
    };
};

struct TsFbxShapeVertex
{
    TsInt       index;
    TsFloat3    pos;
};

struct TsFbxBlendShapeKey
{
    TsF32       time;
    TsF32       weight;
    TsInt       beginIndex;
    TsInt       endIndex;
};

#define TsFbxShapeArray TsVector<TsFbxShapeVertex>

#include "TsFbxContext.h"
#include "TsFbxNode.h"
#include "TsFbxBone.h"
#include "TsFbxBlendShape.h"
#include "TsFbxMesh.h"
#include "TsFbxMaterial.h"

#include "TsFbxScene.h"
#include "TsFbxSkinAnimation.h"
