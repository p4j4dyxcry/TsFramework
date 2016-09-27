//!*******************************************************
//!	TsTransForm.h
//!
//!	トランスフォームを定義するクラス
//!
//!	著作権	2016 Yuki Tsuneyama
//!	作成日時	16.01.13
#pragma once

#include "../../\Extern/xna/Xna.h"
#include "TsVector.h"
#include "TsQuaternion.h"
#include "TsMatrix.h"

class TsTransForm : TsNameObject
{
public:
	TsTransForm() :m_parent( nullptr ),
				  m_localScale(1,1,1){};
	TsTransForm( const TsVector3& translate ,
				const TsQuaternion quaternion ,
				const TsVector3 scale = TsVector3::one )
				:m_parent( nullptr ) ,				
				 m_localPosition( translate ) ,
				 m_localRotate( quaternion ) ,
				 m_localScale( scale ){};
	virtual ~TsTransForm();
	TsTransForm( const TsMatrix& m );

	TsMatrix	ToWorldMatrix();
	TsMatrix	ToLocalMatrix();
	TsTransForm* GetParent()const;
	void SetParent( __inout TsTransForm* transform );
	void AddChild( TsTransForm* transform );
	TsVector3		m_localPosition;
	TsQuaternion	m_localRotate;
	TsVector3		m_localScale;

	TsTransForm& operator = ( const TsMatrix& m );
	TsTransForm  operator * ( const TsMatrix& m );
	TsTransForm& operator *= ( const TsMatrix& m );
	TsTransForm* FindChildByName(const TsString&);
	TsTransForm* FindChildByhash(TS_HASH);
protected:
	TsTransForm*		m_parent		= nullptr;
	TsTransForm*		m_firstChild	= nullptr;
	TsTransForm*		m_subling		= nullptr;
private:
	//! 親子関係の解除
	TsBool	RemoveOfParantChild( );
};
