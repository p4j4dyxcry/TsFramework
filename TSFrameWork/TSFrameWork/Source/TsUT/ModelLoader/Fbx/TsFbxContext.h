#pragma once

class TsFbxScene;
class TsFbxContext
{
public:
    TsFbxContext();
    virtual ~TsFbxContext();
    TsBool Initialize();

    FbxManager* GetFbxManager()const;
    TsBool LoadFBX( const TsChar * );
    TsFbxScene* GetSceneByIndex( TsInt index );
    TsFbxScene* GetSceneByFileName( const TsString& name );
    FbxTime::EMode GetTimeLocale();
private:
    TsFbxScene* CreateScene( const TsString& filename );

    FbxManager*             m_pFbxManager;
    FbxTime::EMode          m_fbxTimeLocation;
    TsVector<TsFbxScene*>   m_pFbxScenes;
};

class TsFbxObject
{
public:
    TsFbxObject( TsFbxContext* pFbxContext , TsFbxScene* pFbxScene)
    {
        m_pFbxContext = pFbxContext;
        m_pFbxScene = pFbxScene;
    }
    virtual ~TsFbxObject(){};
protected:
    TsFbxContext* m_pFbxContext;
    TsFbxScene *  m_pFbxScene;
};