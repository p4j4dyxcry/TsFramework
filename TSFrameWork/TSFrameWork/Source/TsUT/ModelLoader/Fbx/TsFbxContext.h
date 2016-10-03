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
	TsFbxScene* GetSceneByFileName( const TsString& name );
private:
	TsFbxScene* CreateScene( const TsString& filename );

	FbxManager*				m_pFbxManager;
	FbxImporter*			m_pFbxImporter;
	TsVector<TsFbxScene*>   m_pFbxScenes;
};

class TsFbxObject
{
public:
	TsFbxObject( TsFbxContext* pFbxContext )
	{
		m_pFbxContext = pFbxContext;
	}
	virtual ~TsFbxObject(){};
protected:
	TsFbxContext* m_pFbxContext;
};