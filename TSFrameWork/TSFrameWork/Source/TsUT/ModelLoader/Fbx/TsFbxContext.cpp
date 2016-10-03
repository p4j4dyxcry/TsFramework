#include "../../TsUT.h"
#include "TsFbxHeader.h"

TsFbxContext::TsFbxContext()
	: m_pFbxManager( nullptr ),
	 m_pFbxImporter(nullptr)
{

}

TsFbxContext::~TsFbxContext()
{
	FbxSafeRelease( m_pFbxImporter );
	FbxSafeRelease( m_pFbxManager );
}



TsBool TsFbxContext::Initialize()
{
	m_pFbxManager = FbxManager::Create();
	if( m_pFbxManager == nullptr ) 
		return TS_FALSE;

	m_pFbxImporter = FbxImporter::Create( m_pFbxManager , "imp" );
	if( m_pFbxImporter == nullptr )
		return TS_FALSE;

	return TS_TRUE;
}

TsBool TsFbxContext::LoadFBX( const TsChar * filename )
{
	if( m_pFbxManager == nullptr )
	{
		TsBool isInitialize = Initialize();
		if( isInitialize == TS_FALSE )
			return TS_FALSE;
	}

	m_pFbxImporter->Initialize( filename );
	FbxScene* pFbxScene = FbxScene::Create( m_pFbxManager , filename );
	TsBool bIsImp = m_pFbxImporter->Import( pFbxScene );

	if( bIsImp == TS_FALSE )
		return TS_FALSE;

	TsFbxScene* pScene = TsNew( TsFbxScene( this ) );
	pScene->BindFbxScene( pFbxScene );

	return TS_TRUE;
}

FbxManager* TsFbxContext::GetFbxManager()const
{
	return m_pFbxManager;
}
