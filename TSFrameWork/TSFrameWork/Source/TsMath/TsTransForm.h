//!*******************************************************
//!	TsTransForm.h
//!
//!	トランスフォームクラスを定義
//!
//!	著作権	2016 Yuki Tsuneyama
//!	作成日時	16.01.13
#pragma once

#include "../../\Extern/xna/Xna.h"
#include "TsVector.h"
#include "TsQuaternion.h"
#include "TsMatrix.h"

//!========================================================
// 概要:行列から位置、回転、拡縮を切り離し、親子関係を管理
//		できるようにしたクラス
//!========================================================
class TsTransForm : TsNameObject
{
public:
	//! 規定のコンストラクタ
	TsTransForm() :m_parent( nullptr ),
				  m_localScale(1,1,1){};

	//! 初期化された規定のTransformを作成
	//  translate	位置
	//  quaternion	回転成分
	//	scale		拡縮(デフォルトは1,1,1)
	TsTransForm( const TsVector3& translate ,
				const TsQuaternion quaternion ,
				const TsVector3 scale = TsVector3::one )
				:m_parent( nullptr ) ,				
				 m_localPosition( translate ) ,
				 m_localRotate( quaternion ) ,
				 m_localScale( scale ){};

	//!デストラクタ
	virtual ~TsTransForm();

	//! コピーコンストラクタ
	TsTransForm( const TsMatrix& m );

	//! TransformをWorldSpaceMatrixで取得する
	TsMatrix	ToWorldMatrix();

	//! TransformをLocalSpaceMatrixで取得する
	TsMatrix	ToLocalMatrix();

	//! ワールド座標上の位置を祝する
	TsVector3	GetWorldPos();

	TsQuaternion GetWorldRotate();

	//! ワールド空間でのスケールを取得する
	TsVector3	GetWorldScale();

	//! 親のTransformを取得する
	//	親がいない場合はNULLが返る
	TsTransForm* GetParent()const;

	//! 親を設定する。このメソッドで設定されたTransformは
	//	関連する親子関係の全情報を書き換えることがある。
	TsBool SetParent(__inout TsTransForm* transform);

	//! 子ノードを追加する。このメソッドで設定されたTransformは
	//	関連する親子関係の全情報を書き換えることがある。
	TsBool AddChild( TsTransForm* transform );

	//! 子ノードを名前で探索し一致するTransformを取得する
	//　一致するTransformが存在しない場合NULLが返る
	TsTransForm* FindChildByName(const TsString&);

	//! 子ノードをハッシュで探索し一致するTransformを取得する
	//　一致するTransformが存在しない場合NULLが返る
	TsTransForm* FindChildByhash(TS_HASH);

	//! LocalMatrixからTransformに変換する。
	TsTransForm& operator = (const TsMatrix& m);

	//! LocalMatrixと乗算した結果を取得する
	TsTransForm  operator * (const TsMatrix& m);

	//! LocalMatrixと乗算する
	TsTransForm& operator *= (const TsMatrix& m);

public:
	TsVector3		m_localPosition;				//! ローカル位置
	TsQuaternion	m_localRotate;					//! ローカル回転
	TsVector3		m_localScale;					//! ローカルスケール
protected:
	TsTransForm*		m_parent		= nullptr;	//! 親へのポインタ
	TsTransForm*		m_firstChild	= nullptr;	//! 第一子へのポインタ
	TsTransForm*		m_subling		= nullptr;	//! 兄弟へのポインタ
private:

	//! 親子関係を解除する
	//	このメソッドは関連する親子関係の全情報を書き換えることがある。
	TsBool	RemoveOfParantChild( );
};
