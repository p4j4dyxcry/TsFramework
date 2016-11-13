#include "TsMath.h"

const TsVector3 TsVector3::zero = TsVector3( 0 , 0 , 0);
const TsVector3 TsVector3::one  = TsVector3( 1 , 1 , 1);
const TsVector3 TsVector3::up   = TsVector3( 0 , 1 , 0);
const TsVector3 TsVector3::down = TsVector3( 0 ,-1 , 0 );
const TsVector3 TsVector3::left = TsVector3(-1 , 0 , 0 );
const TsVector3 TsVector3::right= TsVector3( 1 , 0 , 0 );
const TsVector3 TsVector3::front= TsVector3( 0 , 0 , 1 );
const TsVector3 TsVector3::back = TsVector3( 0 , 0 ,-1 );

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

TsVector3 TsVector3::operator/ (const XMFLOAT3& v)const
{	
    return TsVector3( x/v.x, y/v.y, z/v.z);
}

TsVector3 TsVector3::operator* (const XMFLOAT3& v)const
{
    return TsVector3(x * v.x, y * v.y, z * v.z);
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
TsVector3& TsVector3::operator*= (const XMFLOAT3& v)
{
    return *this = *this * v;
}
TsVector3& TsVector3::operator/= (const XMFLOAT3& v)
{
    return *this = *this / v;
}

TsVector3  TsVector3::operator*( const TsMatrix& mat )const
{
    return TsVector3( XMVector3Transform( ToXMVECTOR() , mat.ToXMMATRIX() ) );
}

TsVector3 operator * ( TsF32 f , const class TsVector3& v )
{
    return v * f;
}

TsVector3& TsVector3::operator*=( const TsMatrix& mat )
{
    return *this = *this * mat;
}

TsBool TsVector3::operator==( const TsVector3& value )const
{
    return	abs(x - value.x) < 0.00001f  &&
            abs(y - value.y) < 0.00001f &&
            abs(z - value.z) < 0.00001f;
}
TsBool TsVector3::operator!=( const TsVector3& value )const
{
    return !( *this == value );
}
TsBool TsVector3::operator>=( const TsVector3& value )const
{
    return x >= value.x && y >= value.y && z>=value.z;
}
TsBool TsVector3::operator<=( const TsVector3& value )const
{
    return x <= value.x && y <= value.y && z <= value.z;
}
TsBool TsVector3::operator>( const TsVector3& value )const
{
    return x > value.x && y > value.y && z > value.z;
}
TsBool TsVector3::operator<( const TsVector3& value )const
{
    return x < value.x && y < value.y && z >value.z;
}

TsVector3  TsVector3::operator - ( )const
{
    return *this * -1; 
}
TsVector3& TsVector3::TransformCood( const TsMatrix& mat )
{
    return 	*this = XMVector3TransformCoord( ToXMVECTOR() , mat.ToXMMATRIX() );
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
    TsVector3 v = TsVector3( XMVector3Normalize( ToXMVECTOR() ) );
    v.x = abs( v.x ) < 0.000001f ? 0 : v.x;
    v.y = abs( v.y ) < 0.000001f ? 0 : v.y;
    v.z = abs( v.z ) < 0.000001f ? 0 : v.z;
    return v;
}

FLOAT TsVector3::Dot( TsVector3 v0 ,
                  TsVector3 v1 )
{
    return XMVector3Dot( v0.ToXMVECTOR() , v1.ToXMVECTOR() ).m128_f32[0];
}

TsVector3 TsVector3::Cross( TsVector3 v0 ,
                       TsVector3 v1 )
{
    return TsVector3( XMVector3Cross( v0.ToXMVECTOR() , v1.ToXMVECTOR() ) );
}

TsF32 TsVector3::CrossingAngle( TsVector3 v0 , TsVector3 v1 )
{
    float d = v0.Length() * v1.Length();

    if( d == 0 )return 0;

    TsF32 c = Dot( v0 , v1 ) / d;

    if( c >=  1.0 ) return 0;
    if( c <= -1.0 ) return TS_PI;

    return acos(c);
}

