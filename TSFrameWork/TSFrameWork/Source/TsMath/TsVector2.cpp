#include "TsVector.h"

const TsVector2 TsVector2::zero = TsVector2( 0, 0);
const TsVector2 TsVector2::one  = TsVector2( 1, 1);
const TsVector2 TsVector2::up   = TsVector2( 0, 1);
const TsVector2 TsVector2::down = TsVector2( 0,-1);
const TsVector2 TsVector2::left = TsVector2(-1, 0);
const TsVector2 TsVector2::right= TsVector2( 1, 0);

TsVector2& TsVector2::operator = ( const XMVECTOR& Float2 )
{
    this->x = Float2.m128_f32[0];
    this->y = Float2.m128_f32[1];
    return *this;
}

TsVector2  TsVector2::operator+ ( const XMFLOAT2& Float2 )const
{
    return TsVector2( x + Float2.x , y + Float2.y );
}
TsVector2  TsVector2::operator- ( const XMFLOAT2& Float2 )const
{
    return TsVector2( x - Float2.x , y - Float2.y );
}
TsVector2  TsVector2::operator* ( FLOAT Float )const
{
    return TsVector2( x*Float , y*Float );
}
TsVector2 operator * ( TsF32 f , const class TsVector2& v )
{
    return v * f;
}
TsVector2  TsVector2::operator/ ( FLOAT Float )const
{
    return TsVector2( x/Float , y/Float );
}
TsVector2& TsVector2::operator+= ( const XMFLOAT2& Float2 )
{
    return *this = *this + Float2;
}
TsVector2& TsVector2::operator-= ( const XMFLOAT2& Float2 )
{
    return *this = *this - Float2;
}
TsVector2& TsVector2::operator*= ( FLOAT Float )
{
    return *this = *this * Float;
}
TsVector2& TsVector2::operator/= ( FLOAT Float )
{
    return *this = *this / Float;
}
TsBool TsVector2::operator==( const TsVector2& value )const
{
    return x == value.x &&
        y == value.y;
}
TsBool TsVector2::operator!=( const TsVector2& value )const
{
    return !( *this == value );
}
TsBool TsVector2::operator>=( const TsVector2& value )const
{
    return x >= value.x && y >= value.y;
}
TsBool TsVector2::operator<=( const TsVector2& value )const
{
    return x <= value.x && y <= value.y;
}
TsBool TsVector2::operator>( const TsVector2& value )const
{
    return x > value.x && y > value.y;
}
TsBool TsVector2::operator<( const TsVector2& value )const
{
    return x < value.x && y < value.y;
}

TsVector2 TsVector2::operator* (const TsMatrix& matrix)
{
    return TsVector2(XMVector2Transform(ToXMVECTOR(),matrix.ToXMMATRIX()));
}

TsVector2& TsVector2::operator*= (const TsMatrix& matrix)
{
    return *this = *this * matrix;
}


TsVector2  TsVector2::operator - ( )const
{
    return *this * -1;
}

XMVECTOR TsVector2::ToXMVECTOR()const
{
    return XMLoadFloat2( this );
}

FLOAT TsVector2::Length ()const
{
    XMVECTOR vLength = XMVector2Length( ToXMVECTOR() );
    return ( vLength.m128_f32[0] );
}

TsVector2& TsVector2::Normalize()
{
    *this = Normalized();
    return *this;
}
TsVector2 TsVector2::Normalized()const
{
    return TsVector2( XMVector2Normalize( ToXMVECTOR() ));
}


FLOAT TsVector2::Dot(	TsVector2 v0 ,
                        TsVector2 v1 )
{
    return XMVector2Dot( v0.ToXMVECTOR() , v1.ToXMVECTOR() ).m128_f32[0];
}

TsVector2 TsVector2::Cross(	TsVector2 v0 ,
                            TsVector2 v1 )
{
    return TsVector2 ( XMVector2Cross( v0.ToXMVECTOR() , v1.ToXMVECTOR() ) );
}

TsF32 TsVector2::Sum()const
{
    return x + y;
}
