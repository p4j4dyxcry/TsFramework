#include "../../TsUT.h"
#include "TsFbxLoader.h"
#include "TsFbxHeader.h"

TsFbxLoader::TsFbxLoader()
{
    m_pFbxContext = TsNew( TsFbxContext );
}
TsFbxLoader::~TsFbxLoader()
{
    TsSafeDelete( m_pFbxContext );
}

//! Load From File
TsBool TsFbxLoader::LoadFromFile( const TsString& filename )
{
    return m_pFbxContext->LoadFBX( filename.c_str() );
}

//! Load From Memory
TsBool TsFbxLoader::LoadFromMemory( void* memory , size_t sz )
{
    ( void )memory;
    ( void )sz;
    return TS_FALSE;
}

//! Get Mesh Num
TsInt  TsFbxLoader::GetMeshNum()
{
    return m_pFbxContext->GetSceneByIndex( 0 )->GetMeshNum();
}

//! Get Vertex Size by byte
TsInt  TsFbxLoader::GetVertexSize( TsInt index )
{
    auto meshList = m_pFbxContext->GetSceneByIndex( 0 )->GetMeshList();
    return GetVertexStride() * meshList[index]->GetVertexCount();
}

//! Get Vertex Buffer by byte
void*  TsFbxLoader::GetVertexBuffer( TsInt index )
{
    auto meshList = m_pFbxContext->GetSceneByIndex( 0 )->GetMeshList();
    return meshList[index]->CreateVertexBuffer();
}

//! Get Vertex Stride
size_t TsFbxLoader::GetVertexStride()
{
    auto meshList = m_pFbxContext->GetSceneByIndex( 0 )->GetMeshList();
    return meshList[0]->GetVertexStride();
}

//! GetIndexBuffer
void*  TsFbxLoader::GetIndexBuffer(TsInt index)
{
    auto meshList = m_pFbxContext->GetSceneByIndex(0)->GetMeshList();
    return meshList[index]->CreateIndexBuffer();
}

//! GetIndexBufferSize
size_t TsFbxLoader::GetIndexBufferSize( TsInt index )
{
    auto meshList = m_pFbxContext->GetSceneByIndex(0)->GetMeshList();
    return meshList[index]->GetIndexBufferSize();
}

TsTransForm* TsFbxLoader::GetTransform( TsInt index )const
{
    auto meshList = m_pFbxContext->GetSceneByIndex( 0 )->GetMeshList();
    return meshList[index]->GetTransform();
}

//! GetIndexBuffer
TsString  TsFbxLoader::GetAlbedoTexturePass( TsInt index )
{
    auto&& meshList = m_pFbxContext->GetSceneByIndex( 0 )->GetMeshList();
    TsInt materialIndex = meshList[index]->GetMaterialIndex();
    TsFbxMaterial material = m_pFbxContext->GetSceneByIndex( 0 )->GetMaterialList()[materialIndex];
    return material.GetAlbedoTextureName().GetFileName();
}