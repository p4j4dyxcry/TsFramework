//!*******************************************************
//!	Material.h
//!
//!	�}�e���A���t�H�[�}�b�g�̒�`
//!
//!	���쌠	2016 Yuki Tsuneyama
//!	�쐬����	16.01.15

#pragma once

#include "CColor.h"
#include <string>

class CMaterialBase
{
protected:
	bool m_isBumpMap;
	bool m_isSpecularMap;
public:
	CColor m_base;			//�x�[�X

	std::string m_baseTex;		//�f�J�[��
	std::string m_normalTex;	//�m�[�}��
	std::string m_specularTex;	//�X�y�L����

	std::string m_materialName;
};

class CLambartMaterial : public CMaterialBase
{
public:
	CColor m_ambient;
	CColor m_emissiv;
};

class CBlinnPhongMaterial : public CMaterialBase
{
	//TODO ������
};

class CCookTorranceMaterial : public CMaterialBase
{
	//TODO ������
};
class CPBLMaterial : public CMaterialBase
{
public:
	FLOAT m_roughness;		//���t�l�X
	FLOAT m_metallic;		//���^���b�N
	FLOAT m_specular;		//�X�y�L����
};

/*
//! PBL BaseColor(����)
�ؒY	0.02
�V�����A�X�t�@���g	0.02
���Ղ����A�X�t�@���g	0.08
�I�o�y��	0.13
��	0.21
�����̍�	0.36
�V�����R���N���[�g	0.51
�X��	0.56
�V��	0.81

�S	(0.560, 0.570, 0.580)
��	(0.972, 0.960, 0.915)
�A���~�j�E��	(0.913, 0.921, 0.925)
��	(1.000, 0.766, 0.336)
��	(0.955, 0.637, 0.538)
�N����	(0.550, 0.556, 0.554)
�j�b�P��	(0.660, 0.609, 0.526)
�`�^��	(0.542, 0.497, 0.449)
�R�o���g	(0.662, 0.655, 0.634)
�v���`�i	(0.672, 0.637, 0.585)

*/
