#include "../../TsUT.h"
#include "TsFbxHeader.h"

TsFbxContext::TsFbxContext()
    :m_pFbxManager( nullptr ),
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
    m_fbxTimeLocation = FbxTime::eFrames60;

    return TS_TRUE;
}

FbxTime::EMode TsFbxContext::GetTimeLocale()
{
    return m_fbxTimeLocation;
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
    if (pFbxScene)
    {
        FbxGlobalSettings& globalSetting = pFbxScene->GetGlobalSettings();

        auto axis = globalSetting.GetAxisSystem();

        //globalSetting.SetAxisSystem(FbxAxisSystem::DirectX);
        FbxAxisSystem system(FbxAxisSystem::eMayaYUp);
        system.ConvertScene(pFbxScene);
        //globalSetting.SetSystemUnit(FbxSystemUnit::m);
    }

    if( bIsImp == TS_FALSE )
        return TS_FALSE;

    TsFbxScene* pScene = TsNew( TsFbxScene( this ) );
    pScene->BindFbxScene( pFbxScene );
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