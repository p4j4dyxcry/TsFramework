#include "TsVector.h"
#include "TsMatrix.h"
const TsVector4 TsVector4::zero = TsVector4( 0 , 0 , 0 ,0 );
const TsVector4 TsVector4::one  = TsVector4( 1 , 1 , 1 ,1);
const TsVector4 TsVector4::up   = TsVector4( 0 , 1 , 0 ,0);
const TsVector4 TsVector4::down = TsVector4( 0 , -1 , 0,0 );
const TsVector4 TsVector4::left = TsVector4( -1 , 0 , 0,0 );
const TsVector4 TsVector4::right= TsVector4( 1 , 0 , 0 ,0);
const TsVector4 TsVector4::front= TsVector4( 0 , 0 , 1 ,0);
const TsVector4 TsVector4::back = TsVector4( 0 , 0 , -1 ,0);

TsVector4& TsVector4::operator = ( const XMVECTOR& Float4 )
{
    this->x = Float4.m128_f32[0];
    this->y = Float4.m128_f32[1];
    this->z = Float4.m128_f32[2];
    this->w = Float4.m128_f32[3];
    return *this;
}

TsVector4& TsVector4::operator= ( const XMFLOAT3& Float3 )
{
    this->x = Float3.x;
    this->y = Float3.y;
    this->z = Float3.z;
    this->w = 0;
    return *this ;
}

TsVector4  TsVector4::operator+ ( const XMFLOAT4& Float4 )const
{
    return TsVector4( x + Float4.x , y + Float4.y , z + Float4.z , w + Float4.w );
}
TsVector4  TsVector4::operator- ( const XMFLOAT4& Float4 )const
{
    return TsVector4( x - Float4.x , y - Float4.y , z - Float4.z , w - Float4.w );
}
TsVector4  TsVector4::operator* ( FLOAT Float )const
{
    return TsVector4( x * Float , y * Float , z * Float , w * Float );
}
TsVector4  TsVector4::operator/ ( FLOAT Float )const
{
    return TsVector4( x / Float , y / Float , z / Float , w / Float );
}

TsVector4& TsVector4::operator+= ( const XMFLOAT4& Float4 )
{
    x += Float4.x;
    y += Float4.y;
    z += Float4.z;
    w += Float4.w;
    return *this;
}
TsVector4& TsVector4::operator-= ( const XMFLOAT4& Float4 )
{
    x -= Float4.x;
    y -= Float4.y;
    z -= Float4.z;
    w -= Float4.w;
    return *this;
}
TsVector4& TsVector4::operator*= ( FLOAT Float )
{
    x *= Float;
    y *= Float;
    z *= Float;
    w *= Float;
    return *this;
}
TsVector4& TsVector4::operator/= ( FLOAT Float )
{
    x /= Float;
    y /= Float;
    z /= Float;
    w /= Float;
    return *this;
}

TsVector4  TsVector4::operator*( const TsMatrix& mat )
{
    return TsVector4( XMVector4Transform( ToXMVECTOR() , mat.ToXMMATRIX() ) );
}

TsVector4 operator * ( TsF32 f , const class TsVector4& v )
{
    return v * f;
}

TsVector4& TsVector4::operator*=( const TsMatrix& mat )
{
    return *this = *this * mat;
}
TsBool TsVector4::operator==( const TsVector4& value )const
{
    return	x == value.x &&
        y == value.y &&
        z == value.z && 
        w == value.w;
}
TsBool TsVector4::operator!=( const TsVector4& value )const
{
    return !( *this == value );
}
TsBool TsVector4::operator>=( const TsVector4& value )const
{
    return x >= value.x && y >= value.y && z >= value.z;
}
TsBool TsVector4::operator<=(const TsVector4& value )const
{
    return x <= value.x && y <= value.y && z <= value.z;
}
TsBool TsVector4::operator>(const TsVector4& value )const
{
    return x > value.x && y > value.y && z > value.z;
}
TsBool TsVector4::operator<(const TsVector4& value )const
{
    return x < value.x && y < value.y && z >value.z;
}

TsVector4  TsVector4::operator - ( )const
{
    return *this * -1;
}

XMVECTOR TsVector4::ToXMVECTOR()const
{
    return XMLoadFloat4( this );
}

FLOAT TsVector4::Length()const
{
    XMVECTOR vLength = XMVector2Length( ToXMVECTOR() );
    return ( vLength.m128_f32[0]);
}

TsVector4& TsVector4::Normalize()
{
    *this = Normalized();
    return *this;
}
TsVector4 TsVector4::Normalized()const
{
    return TsVector4( XMVector4Normalize( ToXMVECTOR() ) );
}

FLOAT TsVector4::Dot( TsVector4 v0 ,
                  TsVector4 v1 )
{
    return XMVector4Dot( v0.ToXMVECTOR() , v1.ToXMVECTOR() ).m128_f32[0];
}

TsVector4 TsVector4::Closs( TsVector4 v0 ,
                       TsVector4 v1 )
{
    return TsVector3( XMVector3Cross( v0.ToXMVECTOR() , v1.ToXMVECTOR() ) );
}