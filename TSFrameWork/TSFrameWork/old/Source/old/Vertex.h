//!*******************************************************
//!	Vertex.h
//!
//!	頂点フォーマットの定義
//!
//!	著作権	2016 Yuki Tsuneyama
//!	作成日時	16.01.15

#pragma once

#include <d3d11.h>

#include "Common.h"
#include "CVector.h"
#include "CColor.h"
#include "ArrayType.h"

// ボーン情報
struct ModelBoneWeight
{
	Int4		m_boneIndex;				// ボーンインデックス番号
	Vector4		m_boneWeight;					// ボーンウエイト値
	ModelBoneWeight()
		:m_boneIndex( 0 , 0 , 0 , 0 ),
		m_boneWeight(1 , 0 , 0 , 0){}
};
//!*******************************************************
//!	[クラス] メッシュカスタム頂点クラス
//!
class CMeshVertex
{
public:
	CMeshVertex() :m_color( 1 , 1 , 1 , 1 ){};

	bool operator == ( const CMeshVertex& v ) const{		//　＝＝演算子のオーバーロード 
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
//!	[クラス] スキンメッシュカスタム頂点クラス
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