//!*******************************************************
//!	Material.h
//!
//!	マテリアルフォーマットの定義
//!
//!	著作権	2016 Yuki Tsuneyama
//!	作成日時	16.01.15

#pragma once

#include "CColor.h"
#include <string>

class CMaterialBase
{
protected:
	bool m_isBumpMap;
	bool m_isSpecularMap;
public:
	CColor m_base;			//ベース

	std::string m_baseTex;		//デカール
	std::string m_normalTex;	//ノーマル
	std::string m_specularTex;	//スペキュラ

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
	//TODO 未実装
};

class CCookTorranceMaterial : public CMaterialBase
{
	//TODO 未実装
};
class CPBLMaterial : public CMaterialBase
{
public:
	FLOAT m_roughness;		//ラフネス
	FLOAT m_metallic;		//メタリック
	FLOAT m_specular;		//スペキュラ
};

/*
//! PBL BaseColor(明暗)
木炭	0.02
新しいアスファルト	0.02
摩耗したアスファルト	0.08
露出土壌	0.13
草	0.21
砂漠の砂	0.36
新しいコンクリート	0.51
氷河	0.56
新雪	0.81

鉄	(0.560, 0.570, 0.580)
銀	(0.972, 0.960, 0.915)
アルミニウム	(0.913, 0.921, 0.925)
金	(1.000, 0.766, 0.336)
銅	(0.955, 0.637, 0.538)
クロム	(0.550, 0.556, 0.554)
ニッケル	(0.660, 0.609, 0.526)
チタン	(0.542, 0.497, 0.449)
コバルト	(0.662, 0.655, 0.634)
プラチナ	(0.672, 0.637, 0.585)

*/
