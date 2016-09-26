#include "../TsUT/TsUT.h"

#include "TsTransForm.h"
#include "TsMatrix.h"


using namespace std;

TsTransForm::TsTransForm(const TsMatrix& m)
{
	this->TsTransForm::TsTransForm();
	*this = m;
}

TsTransForm& TsTransForm::operator=( const TsMatrix& m )
{
	TsVector3 xA( m.m[0][0] , m.m[0][1] , m.m[0][2] );
	TsVector3 yA( m.m[1][0] , m.m[1][1] , m.m[1][2] );
	TsVector3 zA( m.m[2][0] , m.m[2][1] , m.m[2][2] );

	float xScale = xA.Length();
	float yScale = yA.Length();
	float zScale = zA.Length();

	m_localScale = TsVector3( xScale , yScale , zScale );
	m_localPosition = TsVector3( m.m[3][0] , m.m[3][1] , m.m[3][2] );

	TsMatrix rotate;

	rotate.m[0][0] = xA.x / xScale;
	rotate.m[1][0] = xA.y / xScale;
	rotate.m[2][0] = xA.z / xScale;

	rotate.m[0][1] = yA.x / yScale;
	rotate.m[1][1] = yA.y / yScale;
	rotate.m[2][1] = yA.z / yScale;

	rotate.m[0][2] = zA.x / zScale;
	rotate.m[1][2] = zA.y / zScale;
	rotate.m[2][2] = zA.z / zScale;

	m_localRotate = rotate.ToQuaternion();

	return *this;
}

TsTransForm TsTransForm::operator*( const TsMatrix& m )
{
	TsMatrix local = ToLocalMatrix();
	TsTransForm result( ( local * m ) );
	result.m_name = m_name;
	result.m_parent = m_parent;
	return result;
}

TsTransForm& TsTransForm::operator*=( const TsMatrix& m )
{
	return *this = (*this * m);
}
//!*******************************************************
//!	[メソッド] ローカル行列を取得する
TsMatrix TsTransForm::ToLocalMatrix()
{
	return TsMatrix( m_localPosition , m_localRotate ,m_localScale);
}

//!*******************************************************
//!	[メソッド] ワールド行列を取得する
TsMatrix TsTransForm::ToWorldMatrix()
{
	TsMatrix result;
	TsTransForm* temp = this;

	TsStack<TsMatrix> matStack;

	//親を辿ってローカル行列をスタックに格納する
	do 
	{
		matStack.push( temp->ToLocalMatrix() );
		temp = temp->m_parent;
	} while( temp != nullptr );

	//スタックからローカル行列を取り出しワールド行列を計算する
	while( matStack.empty() == false )
	{
		result *= matStack.top();
		matStack.pop();
	}

	return result;
}

TsTransForm* TsTransForm::GetParent()const
{
	return m_parent;
}

void TsTransForm::SetParent( __inout TsTransForm* parent )
{
	if( m_parent )
	{
		
		//todo : 親のサブリングを再構築する。

		return;
	}

	m_parent = parent;

	if( parent->m_firstChild == nullptr )
		parent->m_firstChild = this;
	else
	{

		TsTransForm ** subling = &( m_parent->m_firstChild->m_subling );
		while( *subling != nullptr )
		{
			subling = &( ( *subling )->m_subling );
		}
		*subling = this;
	}
}
