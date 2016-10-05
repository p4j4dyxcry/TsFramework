#include "../TsGfx.h"

TsMeshFactory::TsMeshFactory()
{
	m_pMaterial.reserve( 32 );
	m_pMeshList.reserve( 32 );
}
TsMeshFactory::~TsMeshFactory()
{

}

TsBool TsMeshFactory::LoadFromFile( TsDevice* pDev, TsString filename )
{
	if( filename.rfind( ".mqo" ) != TsString::npos )
	{
		TsMqoLoader loader;
		if( loader.LoadFromFile( filename ) == TS_FALSE )
			return TS_FALSE;

		for( TsInt i = 0; i < loader.GetMeshNum(); ++i )
		{
			TsVertexBuffer* buffer = pDev->CreateVertexBuffer(
				loader.GetVertexBuffer( i ) ,
				loader.GetVertexSize( i ) ,
				loader.GetVertexStride() );

			TsMesh * mesh = TsNew( TsMesh );
			mesh->m_vertexBuffer = buffer;

			TsMaterial* material = TsNew( TsMaterial);
			material->m_textureName = loader.GetTexturePass( i );
			material->LoadTextureFromFile( pDev );

			m_pMaterial.push_back( material );
			m_pMeshList.push_back( mesh );
			m_pTransform.push_back( nullptr );
		}

		return TS_TRUE;
	}

	if( filename.rfind( ".fbx" ) != TsString::npos || 
		filename.rfind( ".FBX" ) != TsString::npos)
	{
		TsFbxLoader loader;
		if( loader.LoadFromFile( filename ) == TS_FALSE )
			return TS_FALSE;

		for( TsInt i = 0; i < loader.GetMeshNum(); ++i )
		{
			TsVertexBuffer* buffer = pDev->CreateVertexBuffer(
				loader.GetVertexBuffer( i ) ,
				loader.GetVertexSize( i ) ,
				loader.GetVertexStride() );

			TsIndexBuffer* indexBuffer = pDev->CreateIndexBuffer(
										loader.GetIndexBuffer(i),
										loader.GetIndexBufferSize(i));

			TsMesh * mesh = TsNew( TsMesh );
			mesh->m_vertexBuffer = buffer;
			mesh->m_indexBuffer = indexBuffer;
			TsMaterial* material = TsNew( TsMaterial );
			//material->m_textureName = loader.GetTexturePass( i );
			//material->LoadTextureFromFile( pDev );
			TsInt id = mesh->GetIndexNum();
			m_pMaterial.push_back( material );
			m_pMeshList.push_back( mesh );
			m_pTransform.push_back( loader.GetTransform( i ) );
		}

		return TS_TRUE;
	}
	return TS_FALSE;
}

TsMaterial* TsMeshFactory::GetMaterial( TsInt index )
{
	return m_pMaterial[index];
}
TsMesh* TsMeshFactory::GetMesh( TsInt index )
{
	return m_pMeshList[index];
}

TsMesh* TsMeshFactory::GetMeshArray()
{
	return nullptr;
}
TsMaterial* TsMeshFactory::GetMaterialArray()
{
	return nullptr;
}

TsGeometryObject* TsMeshFactory::CreateGeometryObject( TsInt index , TsDevice* pDev )
{
	TsGeometryObject * obj = TsNew( TsGeometryObject );

	obj->CreateGeometryObject( pDev , m_pMeshList[index] , m_pMaterial[index] );
	obj->SetTransform( m_pTransform[index] );
	return obj;
}