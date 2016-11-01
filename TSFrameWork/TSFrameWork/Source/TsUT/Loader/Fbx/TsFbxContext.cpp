#include "TsFbxAfx.h"

TsFbxContext::TsFbxContext()
    :m_pFbxManager( nullptr )
{

}

TsFbxContext::~TsFbxContext()
{
    FbxSafeRelease( m_pFbxManager );
}



TsBool TsFbxContext::Initialize()
{
    m_pFbxManager = FbxManager::Create();
    if( m_pFbxManager == nullptr ) 
        return TS_FALSE;

    m_fbxTimeLocation = FbxTime::eFrames60;

    return TS_TRUE;
}

FbxTime::EMode TsFbxContext::GetTimeLocale()
{
    return m_fbxTimeLocation;
}

TsBool TsFbxContext::LoadFBX( const TsChar * filename ,
                              const TsFbxLoadOption & option)
{
    m_loadOption = option;

    if( m_pFbxManager == nullptr )
    {
        TsBool isInitialize = Initialize();
        if( isInitialize == TS_FALSE )
            return TS_FALSE;
    }

    TsFbxScene* pScene = TsNew( TsFbxScene( this ) );
    pScene->LoadFromFile( filename );
    m_pFbxScenes.push_back( pScene );
    return TS_TRUE;
}

FbxManager* TsFbxContext::GetFbxManager()const
{
    return m_pFbxManager;
}

TsFbxScene* TsFbxContext::GetSceneByIndex( TsInt index )
{
    if( m_pFbxScenes.size() > ( unsigned )index )
        return m_pFbxScenes[index];
    else
        return nullptr;
}