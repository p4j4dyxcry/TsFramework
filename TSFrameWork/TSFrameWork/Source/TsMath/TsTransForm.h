//!*******************************************************
//!	TsTransForm.h
//!
//!	�g�����X�t�H�[���N���X���`
//!
//!	���쌠	2016 Yuki Tsuneyama
//!	�쐬����	16.01.13
#pragma once

#include "../../\Extern/xna/Xna.h"
#include "TsVector.h"
#include "TsQuaternion.h"
#include "TsMatrix.h"

//!========================================================
// �T�v:�s�񂩂�ʒu�A��]�A�g�k��؂藣���A�e�q�֌W���Ǘ�
//		�ł���悤�ɂ����N���X
//!========================================================
class TsTransForm : TsNameObject
{
public:
	//! �K��̃R���X�g���N�^
	TsTransForm() :m_parent( nullptr ),
				  m_localScale(1,1,1){};

	//! ���������ꂽ�K���Transform���쐬
	//  translate	�ʒu
	//  quaternion	��]����
	//	scale		�g�k(�f�t�H���g��1,1,1)
	TsTransForm( const TsVector3& translate ,
				const TsQuaternion quaternion ,
				const TsVector3 scale = TsVector3::one )
				:m_parent( nullptr ) ,				
				 m_localPosition( translate ) ,
				 m_localRotate( quaternion ) ,
				 m_localScale( scale ){};

	//!�f�X�g���N�^
	virtual ~TsTransForm();

	//! �R�s�[�R���X�g���N�^
	TsTransForm( const TsMatrix& m );

	//! Transform��WorldSpaceMatrix�Ŏ擾����
	TsMatrix	ToWorldMatrix();

	//! Transform��LocalSpaceMatrix�Ŏ擾����
	TsMatrix	ToLocalMatrix();

	//! ���[���h���W��̈ʒu���j����
	TsVector3	GetWorldPos();

	TsQuaternion GetWorldRotate();

	//! ���[���h��Ԃł̃X�P�[�����擾����
	TsVector3	GetWorldScale();

	//! �e��Transform���擾����
	//	�e�����Ȃ��ꍇ��NULL���Ԃ�
	TsTransForm* GetParent()const;

	//! �e��ݒ肷��B���̃��\�b�h�Őݒ肳�ꂽTransform��
	//	�֘A����e�q�֌W�̑S�������������邱�Ƃ�����B
	TsBool SetParent(__inout TsTransForm* transform);

	//! �q�m�[�h��ǉ�����B���̃��\�b�h�Őݒ肳�ꂽTransform��
	//	�֘A����e�q�֌W�̑S�������������邱�Ƃ�����B
	TsBool AddChild( TsTransForm* transform );

	//! �q�m�[�h�𖼑O�ŒT������v����Transform���擾����
	//�@��v����Transform�����݂��Ȃ��ꍇNULL���Ԃ�
	TsTransForm* FindChildByName(const TsString&);

	//! �q�m�[�h���n�b�V���ŒT������v����Transform���擾����
	//�@��v����Transform�����݂��Ȃ��ꍇNULL���Ԃ�
	TsTransForm* FindChildByhash(TS_HASH);

	//! LocalMatrix����Transform�ɕϊ�����B
	TsTransForm& operator = (const TsMatrix& m);

	//! LocalMatrix�Ə�Z�������ʂ��擾����
	TsTransForm  operator * (const TsMatrix& m);

	//! LocalMatrix�Ə�Z����
	TsTransForm& operator *= (const TsMatrix& m);

public:
	TsVector3		m_localPosition;				//! ���[�J���ʒu
	TsQuaternion	m_localRotate;					//! ���[�J����]
	TsVector3		m_localScale;					//! ���[�J���X�P�[��
protected:
	TsTransForm*		m_parent		= nullptr;	//! �e�ւ̃|�C���^
	TsTransForm*		m_firstChild	= nullptr;	//! ���q�ւ̃|�C���^
	TsTransForm*		m_subling		= nullptr;	//! �Z��ւ̃|�C���^
private:

	//! �e�q�֌W����������
	//	���̃��\�b�h�͊֘A����e�q�֌W�̑S�������������邱�Ƃ�����B
	TsBool	RemoveOfParantChild( );
};
