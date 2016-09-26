//!*******************************************************
//!	CFbxSdk.h
//!
//!	FbxSdkを用いてモデルデータを読み込むヘルパー群
//!
//!	著作権	2016 Yuki Tsuneyama
//!	作成日時	16.01.13

#pragma once

#include <fbxsdk.h>
#include <vector>
#include <unordered_map>

#include "CTransForm.h"
#include "Mesh.h"
#include "CSkinAnimation.h"
#include "Material.h"

#pragma comment(lib,"libfbxsdk-md.lib")

//!*******************************************************
//!	[メソッド] Fbxオブジェクトを安全に開放する
//!
//! 引数
//!		fbxobj	fbxSdkに含まれているクラス
//!*******************************************************
template <typename fbxObj>
inline static void FbxSafeDestory( fbxObj _fbxobj )
{
	if( _fbxobj != nullptr )
		_fbxobj->Destroy();
}

class CFbxMesh;

//!*******************************************************
//!	[クラス] Fbxモデルデータをロードするクラス
//!
//!*******************************************************
class CFbxLoader
{
public:
	CFbxLoader();
	virtual ~CFbxLoader();
	bool Load(const char* filename);
	bool LoadAnimation( const char* filename );

	std::vector<CSingleTransformMesh> GetMeshs()
	{
		return m_meshList;
	}

	std::vector<CSkinAnimation> GetSkinAnimes()
	{
		return m_skinAnimeList;
	}

private :
	FbxManager*				m_pFbxMgr;
	FbxImporter*			m_pFbxImp;
	FbxScene*				m_pFbxScene;

	std::unordered_map<std::string , int>			m_nodeTree;
	std::unordered_map<std::string , int>			m_materialTree;
	std::unordered_map<std::string , int>			m_boneTree;
	std::unordered_map<std::string , CTransForm*>	m_boneMatrixTree;
	std::unordered_map<std::string , CTransForm*>	m_meshMatrixTree;

	std::vector<CSingleTransformMesh>	m_meshList;
	std::vector<CMaterialBase>			m_materialList;
	std::vector<CTransForm*>			m_boneList;

	std::vector<CTransForm*>		m_transformManager;

	std::vector<CSkinAnimation>		m_skinAnimeList;

	CMaterialBase	ParseMaterial( FbxSurfaceMaterial* material );
	CSingleTransformMesh		ParseMesh( FbxMesh* mesh );

	std::vector<int>		GetIndexList( FbxMesh* );
	std::vector<Vector3>	GetPositionList( FbxMesh* mesh ,const	std::vector<int>& indexList );
	std::vector<Vector3>	GetNormalList( FbxMesh* mesh ,	const	std::vector<int>& indexList );
	std::vector<Vector3>	GetTangentList( FbxMesh* mesh , const	std::vector<int>& indexList );
	std::vector<Vector2>	GetUVList( FbxMesh* mesh ,		const	std::vector<int>& indexList );

	void GetWeight( FbxMesh* mesh ,								// ＦＢＸＭＥＳＨ
					const std::vector<int>& indexList ,						// 面の構成情報（頂点インデックス番号）
					std::vector<ModelBoneWeight>& boneWeightList ,			// ウェイト値格納ＶＥＣＴＯＲ
					std::vector<std::string>& boneNodeNameList  );			// 初期姿勢の逆行列格納ＶＥＣＴＯＲ


	CSkinAnimation GetAnimationInfoList( FbxTakeInfo* );

};