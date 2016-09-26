//!*******************************************************
//!	CVector.h
//!
//!	ベクトルクラス群の定義
//!
//!	著作権	2016 Yuki Tsuneyama
//!	作成日時	16.01.13

#pragma once

#include "../../\Extern/xna/Xna.h"
#include "../TsUT/TsUT.h"

class TsVector2;
class TsVector3;
class TsVector4;
class TsQuaternion;
class TsMatrix;

//!*******************************************************
//!	[クラス] Vector2
class TsVector2 : public XMFLOAT2
{
public:
	
	using oprator = XMFLOAT2;

	TsVector2( TsF32 _x = 0, TsF32 _y = 0):XMFLOAT2(_x,_y){};
	TsVector2( XMFLOAT2 vector2 ) :XMFLOAT2( vector2 ){};
	TsVector2( XMFLOAT3 vector3)  :XMFLOAT2(vector3.x , vector3.y){};
	TsVector2( XMFLOAT4 vector4 ) :XMFLOAT2( vector4.x , vector4.y ){};
	TsVector2( XMVECTOR vector ) :XMFLOAT2( vector.m128_f32[0] , vector.m128_f32[1] ){};

	TsVector2& operator= ( const XMVECTOR& Float2 );

	TsVector2  operator+ ( const XMFLOAT2& Float2 )const;
	TsVector2  operator- ( const XMFLOAT2& Float2 )const;
	TsVector2  operator* ( TsF32 Float)const;
	TsVector2  operator/ ( TsF32 Float )const;

	TsVector2& operator+= ( const XMFLOAT2& Float2 );
	TsVector2& operator-= ( const XMFLOAT2& Float2 );
	TsVector2& operator*= ( TsF32 Float );
	TsVector2& operator/= ( TsF32 Float );

	TsBool operator==(const TsVector2& value )const;
	TsBool operator!=(const TsVector2& value )const;
	TsBool operator>=(const TsVector2& value )const;
	TsBool operator<=(const TsVector2& value )const;
	TsBool operator>(const TsVector2& value )const;
	TsBool operator<(const TsVector2& value )const;


	XMVECTOR  ToXMVECTOR()const;
	TsF32	  Length()const;
	TsVector2& Normalize();
	TsVector2  Normalized()const;


	static TsF32 Dot( TsVector2 v0 ,
					  TsVector2 v1 );

	static TsVector2 Closs(	TsVector2 v0 ,
							TsVector2 v1 );

	static const TsVector2 zero;
	static const TsVector2 one;
	static const TsVector2 up;
	static const TsVector2 down;
	static const TsVector2 left;
	static const TsVector2 right;

protected:

};

//!*******************************************************
//!	[クラス] Vector3
class TsVector3 : public XMFLOAT3
{
public:

	using oprator = XMFLOAT3;

	TsVector3( TsF32 _x = 0 , TsF32 _y = 0 , TsF32 _z = 0 ) :XMFLOAT3( _x , _y , _z ){};
	TsVector3( XMFLOAT2 vector2 ) :XMFLOAT3( vector2.x,vector2.y,0 ){};
	TsVector3( XMFLOAT3 vector3 ) :XMFLOAT3( vector3.x , vector3.y ,vector3.z){};
	TsVector3( XMFLOAT4 vector4 ) :XMFLOAT3( vector4.x , vector4.y ,vector4.z){};
	TsVector3( XMVECTOR vector ) :XMFLOAT3( vector.m128_f32[0] , vector.m128_f32[1],vector.m128_f32[2]){};

	TsVector3& operator= ( const XMVECTOR& Float3 );

	TsVector3  operator+ ( const XMFLOAT3& Float3 )const;
	TsVector3  operator- ( const XMFLOAT3& Float3 )const;
	TsVector3  operator* ( TsF32 Float )const;
	TsVector3  operator/ ( TsF32 Float )const;

	TsVector3& operator+= ( const XMFLOAT3& Float3 );
	TsVector3& operator-= ( const XMFLOAT3& Float3 );
	TsVector3& operator*= ( TsF32 Float );
	TsVector3& operator/= ( TsF32 Float );

	TsVector3  operator*( const TsMatrix& mat )const;
	TsVector3& operator*=( const TsMatrix& mat );

	TsBool operator==(const TsVector3& value )const;
	TsBool operator!=(const TsVector3& value )const;
	TsBool operator>=(const TsVector3& value )const;
	TsBool operator<=(const TsVector3& value )const;
	TsBool operator>(const TsVector3& value )const;
	TsBool operator<(const TsVector3& value )const;


	TsVector3& TransformCood(const TsMatrix& mat);
	XMVECTOR  ToXMVECTOR()const;
	TsF32	  Length()const;
	TsVector3& Normalize();
	TsVector3  Normalized()const;


	static TsF32 Dot( TsVector3 v0 ,
					  TsVector3 v1 );

	static TsVector3 Cross( TsVector3 v0 ,
						   TsVector3 v1 );

	static TsF32 CrossingAngle( TsVector3 v0 , TsVector3 v1 );

	static const TsVector3 zero;
	static const TsVector3 one;
	static const TsVector3 up;
	static const TsVector3 down;
	static const TsVector3 left;
	static const TsVector3 right;
	static const TsVector3 front;
	static const TsVector3 back;
	
protected:

};
//!*******************************************************
//!	[クラス] Vector4
class TsVector4 : public XMFLOAT4
{
public:

	using oprator = XMFLOAT4;

	TsVector4( TsF32 _x = 0 , TsF32 _y = 0 , TsF32 _z = 0 ,TsF32 _w = 0) :XMFLOAT4( _x , _y , _z ,_w){};
	TsVector4( XMFLOAT2 vector2 ) :XMFLOAT4( vector2.x , vector2.y , 0 , 0){};
	TsVector4( XMFLOAT3 vector3 ) :XMFLOAT4( vector3.x , vector3.y , vector3.z ,0){};
	TsVector4( XMFLOAT4 vector4 ) :XMFLOAT4( vector4.x , vector4.y , vector4.z ,vector4.w){};
	TsVector4( XMVECTOR vector ) :XMFLOAT4( vector.m128_f32[0] , vector.m128_f32[1] , vector.m128_f32[2],vector.m128_f32[3] ){};

	TsVector4& operator= ( const XMVECTOR& Float4 );
	TsVector4& operator= ( const XMFLOAT3& Float3 );

	TsVector4  operator+ ( const XMFLOAT4& Float4 )const;
	TsVector4  operator- ( const XMFLOAT4& Float4 )const;
	TsVector4  operator* ( TsF32 Float )const;
	TsVector4  operator/ ( TsF32 Float )const;

	TsVector4& operator+= ( const XMFLOAT4& Float4 );
	TsVector4& operator-= ( const XMFLOAT4& Float4 );
	TsVector4& operator*= ( TsF32 Float );
	TsVector4& operator/= ( TsF32 Float );

	TsVector4  operator * ( const TsMatrix& mat );
	TsVector4& operator *=( const TsMatrix& mat );

	TsBool operator==(const TsVector4& value )const;
	TsBool operator!=(const TsVector4& value )const;
	TsBool operator>=(const TsVector4& value )const;
	TsBool operator<=(const TsVector4& value )const;
	TsBool operator>(const TsVector4& value )const;
	TsBool operator<(const TsVector4& value )const;

	XMVECTOR  ToXMVECTOR()const;
	TsF32	  Length()const;
	TsVector4& Normalize();
	TsVector4  Normalized()const;


	static TsF32 Dot( TsVector4 v0 ,
					  TsVector4 v1 );

	static TsVector4 Closs( TsVector4 v0 ,
						   TsVector4 v1 );

	static const TsVector4 zero;
	static const TsVector4 one;
	static const TsVector4 up;
	static const TsVector4 down;
	static const TsVector4 left;
	static const TsVector4 right;
	static const TsVector4 front;
	static const TsVector4 back;

protected:

};