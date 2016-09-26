#include "TsVector.h"
#include "TsMatrix.h"

const TsVector3 TsVector3::zero	= TsVector3( 0 , 0 , 0);
const TsVector3 TsVector3::one	= TsVector3( 1 , 1 , 1);
const TsVector3 TsVector3::up	= TsVector3( 0 , 1 , 0);
const TsVector3 TsVector3::down	= TsVector3( 0 ,-1 , 0 );
const TsVector3 TsVector3::left	= TsVector3(-1 , 0 , 0 );
const TsVector3 TsVector3::right= TsVector3( 1 , 0 , 0 );
const TsVector3 TsVector3::front= TsVector3( 0 , 0 , 1 );
const TsVector3 TsVector3::back	= TsVector3( 0 , 0 ,-1 );

TsVector3& TsVector3::operator = ( const XMVECTOR& Float3 )
{
	this->x = Float3.m128_f32[0];
	this->y = Float3.m128_f32[1];
	this->z = Float3.m128_f32[2];
	return *this;
}

TsVector3  TsVector3::operator+ ( const XMFLOAT3& Float3 )const
{
	return TsVector3( x + Float3.x , y + Float3.y , z + Float3.z );
}
TsVector3  TsVector3::operator- ( const XMFLOAT3& Float3 )const
{
	return TsVector3( x - Float3.x , y - Float3.y , z - Float3.z );
}
TsVector3  TsVector3::operator* ( FLOAT Float )const
{
	return TsVector3( x * Float , y * Float , z * Float );
}
TsVector3  TsVector3::operator/ ( FLOAT Float )const
{
	return TsVector3( x / Float , y / Float , z / Float );
}

TsVector3& TsVector3::operator+= ( const XMFLOAT3& Float3 )
{
	return *this = *this + Float3;
}
TsVector3& TsVector3::operator-= ( const XMFLOAT3& Float3 )
{
	return *this = *this - Float3;
}
TsVector3& TsVector3::operator*= ( FLOAT Float )
{
	return *this = *this * Float;
}
TsVector3& TsVector3::operator/= ( FLOAT Float )
{
	return *this = *this / Float;
}

TsVector3  TsVector3::operator*( const TsMatrix& mat )
{
	return TsVector3( XMVector3Transform( ToXMVECTOR() , mat.ToXMMATRIX() ) );
}
TsVector3& TsVector3::operator*=( const TsMatrix& mat )
{
	return *this = *this * mat;
}

XMVECTOR TsVector3::ToXMVECTOR()const
{
	return XMLoadFloat3( this );
}

FLOAT TsVector3::Length()const
{
	XMVECTOR vLength = XMVector3Length( ToXMVECTOR() );
	return ( vLength.m128_f32[0] );
}

TsVector3& TsVector3::Normalize()
{
	*this = Normalized();
	return *this;
}
TsVector3 TsVector3::Normalized()const
{
	return TsVector3( XMVector3Normalize( ToXMVECTOR() ) );
}

FLOAT TsVector3::Dot( TsVector3 v0 ,
				  TsVector3 v1 )
{
	return XMVector3Dot( v0.ToXMVECTOR() , v1.ToXMVECTOR() ).m128_f32[0];
}

TsVector3 TsVector3::Closs( TsVector3 v0 ,
					   TsVector3 v1 )
{
	return TsVector3( XMVector3Cross( v0.ToXMVECTOR() , v1.ToXMVECTOR() ) );
}