//!*******************************************************
//!	Vertex.h
//!
//!	���_�t�H�[�}�b�g�̒�`
//!
//!	���쌠	2016 Yuki Tsuneyama
//!	�쐬����	16.01.15

#pragma once

#include <d3d11.h>

#include "Common.h"
#include "CVector.h"
#include "CColor.h"
#include "ArrayType.h"

// �{�[�����
struct ModelBoneWeight
{
	Int4		m_boneIndex;				// �{�[���C���f�b�N�X�ԍ�
	Vector4		m_boneWeight;					// �{�[���E�G�C�g�l
	ModelBoneWeight()
		:m_boneIndex( 0 , 0 , 0 , 0 ),
		m_boneWeight(1 , 0 , 0 , 0){}
};
//!*******************************************************
//!	[�N���X] ���b�V���J�X�^�����_�N���X
//!
class CMeshVertex
{
public:
	CMeshVertex() :m_color( 1 , 1 , 1 , 1 ){};

	bool operator == ( const CMeshVertex& v ) const{		//�@�������Z�q�̃I�[�o�[���[�h 
		return std::memcmp( this , &v , sizeof( CMeshVertex ) ) == 0;
	}

	CVector4 m_pos;				//16
	CVector3 m_normal;			//12
	CVector2 m_texture;			//8
	CVector4 m_tangent;			//12
	ModelBoneWeight	m_weight;	//24
	CColor	 m_color;			//16


};
//!*******************************************************
//!	[�N���X] �X�L�����b�V���J�X�^�����_�N���X
//!
/*
class CSkinMeshVertex : public CMeshVertex
{
public:
	CSkinMeshVertex() :CMeshVertex(){}

	Int4   m_boneIndex;
	Float4 m_weight;

};
*/