#include "../../../../TsAfx.h"

Ts3DModelLoader::Ts3DModelLoader()
{
    m_pMeshs = nullptr;
    m_pMaterials = nullptr;
    m_pSkeleton = nullptr;
    m_pTransform = nullptr;
    m_meshCount = 0;
    m_materialCount = 0;
}

Ts3DModelLoader::~Ts3DModelLoader()
{
//    TsSafeDelete(m_pMeshs);
//    TsSafeDelete(m_pMaterials)
}

TsBool Ts3DModelLoader::LoadFile( const TsChar * filename )
{
    SetName( filename );
    TsDebugLog("Load \n\t\"file = %s\"\n",filename);

    return TS_TRUE;
}

TsBool Ts3DModelLoader::SaveFile( const TsChar * filename )
{
    TsDebugLog("Save \n\t\"file = %s\"\n", filename);
    return TS_FALSE;
}

//----------------------------------------------------------
//! メッシュ配列を取得
// @param
// out     pOut メッシュの先頭ポインタ
// return メッシュの数
//----------------------------------------------------------
TsCommon3DMesh* Ts3DModelLoader::GetMeshData()const
{
    return m_pMeshs;
}

//----------------------------------------------------------
//! マテリアル配列を取得
// @param
// out     pOut メッシュの先頭ポインタ
// return メッシュの数
//----------------------------------------------------------
TsCommon3DMaterial* Ts3DModelLoader::GetMaterialData()const
{
    return m_pMaterials;
}

//----------------------------------------------------------
//! スケルトン配列を取得
// @param
// out     pOut メッシュの先頭ポインタ
// return メッシュの数
//----------------------------------------------------------
TsSkeleton* Ts3DModelLoader::GetSkeletonData()const
{
    return m_pSkeleton;
}

//----------------------------------------------------------
//! メッシュ配列を取得
// @param
// out     pOut メッシュの先頭ポインタ
// return メッシュの数
//----------------------------------------------------------
TsTransForm* Ts3DModelLoader::GetRootTransformData( )const
{
    return m_pTransform;
}