//!*******************************************************
//!	TsMatrix.h
//!
//!	行列
//!
//!	著作権	2016 Yuki Tsuneyama
//!	作成日時	16.01.14

#pragma once

#include "../../Extern/xna/Xna.h"

#define Matrix TsMatrix

class TsVector3;
class TsQuaternion;

class TsMatrix : public XMFLOAT4X4
{
public:
	using oprator = XMFLOAT4X4;
	TsMatrix( XMFLOAT4X4& float4x4 ) :XMFLOAT4X4( float4x4 ){};
	TsMatrix(){ *this = TsMatrix::identity; };
	TsMatrix( XMMATRIX& matrix ){ *this = matrix; }
	TsMatrix( TsVector3& pos , TsQuaternion& rotate );
	TsMatrix( TsVector3& pos , TsQuaternion& rotate , TsVector3& scale);

	TsMatrix& operator = ( const XMMATRIX& matrix );
	TsMatrix  operator *  (const TsMatrix& matrix  )const;
	TsMatrix& operator *= (const TsMatrix& matrix );
	XMMATRIX  ToXMMATRIX()const;
	TsQuaternion ToQuaternion()const;
	TsMatrix& Transpose();
	TsMatrix& Inverse();

	TsMatrix Transposed()const;
	TsMatrix Inversed();


	static TsMatrix CreateTranslate( FLOAT x, FLOAT y , FLOAT z );
	static TsMatrix CreateTranslate( const TsVector3& translate );
	static TsMatrix CreateRotate( const TsQuaternion& q );
	static TsMatrix CreateScale( FLOAT x , FLOAT y , FLOAT z );
	static TsMatrix CreateScale( const TsVector3& scale );
	static TsMatrix CreateLookAt( const TsVector3& eye , const TsVector3& at , const TsVector3& up  );

	static const TsMatrix identity;

protected:
};