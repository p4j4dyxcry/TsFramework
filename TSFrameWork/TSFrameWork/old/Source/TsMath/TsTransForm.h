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

#include <unordered_map>
#include <string>
class TsTransForm
{
public:
	std::string m_name;
	TsTransForm() :m_parent( nullptr ),
				  m_localScale(1,1,1){};
	TsTransForm( const TsVector3& translate ,
				const TsQuaternion quaternion ,
				const TsVector3 scale = TsVector3::one )
				:m_parent( nullptr ) ,				
				 m_localPosition( translate ) ,
				 m_localRotate( quaternion ) ,
				 m_localScale( scale ){};
	TsTransForm( const TsMatrix& m );

	TsMatrix	ToWorldMatrix();
	TsMatrix	ToLocalMatrix();
	TsTransForm* GetParent()const;
	void SetParent( __inout TsTransForm* transform );

	TsVector3		m_localPosition;
	TsQuaternion	m_localRotate;
	TsVector3		m_localScale;

	TsTransForm& operator = ( const TsMatrix& m );
	TsTransForm  operator * ( const TsMatrix& m );
	TsTransForm& operator *= ( const TsMatrix& m );
protected:
	TsTransForm*		m_parent		= nullptr;
	TsTransForm*		m_firstChild	= nullptr;
	TsTransForm*		m_subling		= nullptr;
};
