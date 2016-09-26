//!*******************************************************
//!	CFbxSdk.h
//!
//!	FbxSdk��p���ă��f���f�[�^��ǂݍ��ރw���p�[�Q
//!
//!	���쌠	2016 Yuki Tsuneyama
//!	�쐬����	16.01.13

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
//!	[���\�b�h] Fbx�I�u�W�F�N�g�����S�ɊJ������
//!
//! ����
//!		fbxobj	fbxSdk�Ɋ܂܂�Ă���N���X
//!*******************************************************
template <typename fbxObj>
inline static void FbxSafeDestory( fbxObj _fbxobj )
{
	if( _fbxobj != nullptr )
		_fbxobj->Destroy();
}

class CFbxMesh;

//!*******************************************************
//!	[�N���X] Fbx���f���f�[�^�����[�h����N���X
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

	void GetWeight( FbxMesh* mesh ,								// �e�a�w�l�d�r�g
					const std::vector<int>& indexList ,						// �ʂ̍\�����i���_�C���f�b�N�X�ԍ��j
					std::vector<ModelBoneWeight>& boneWeightList ,			// �E�F�C�g�l�i�[�u�d�b�s�n�q
					std::vector<std::string>& boneNodeNameList  );			// �����p���̋t�s��i�[�u�d�b�s�n�q


	CSkinAnimation GetAnimationInfoList( FbxTakeInfo* );

};