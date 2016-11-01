#pragma once

class TsFbxContext
{
public:
    struct TsFbxLoadOption
    {
        TsBool loadAnimation;
        TsBool loadMesh;
        TsBool loadSkeleton;
        TsBool loadTangent;
        TsBool loadBiNormal;
        TsBool loadVertexColor;
        TsBool loadLight;
        TsBool loadCamera;
        TsFbxLoadOption()
            :loadAnimation(TS_TRUE),
            loadMesh( TS_TRUE ) ,
            loadSkeleton( TS_TRUE ) ,
            loadTangent( TS_TRUE ) ,
            loadBiNormal( TS_TRUE ) ,
            loadVertexColor( TS_TRUE ) ,
            loadLight( TS_TRUE ) ,
            loadCamera( TS_TRUE )
        {}
    };
    TsFbxContext();
    virtual ~TsFbxContext();
    TsBool Initialize();

    FbxManager* GetFbxManager()const;
    TsBool LoadFBX( const TsChar * ,const TsFbxLoadOption& option = TsFbxLoadOption() );
    TsFbxScene* GetSceneByIndex( TsInt index );
    TsFbxScene* GetSceneByFileName( const TsString& name );
    FbxTime::EMode GetTimeLocale();

    const TsFbxLoadOption& GetLoadOptin()const
    {
        return m_loadOption;
    }

private:
    TsFbxScene* CreateScene( const TsString& filename );
    TsFbxLoadOption m_loadOption;

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