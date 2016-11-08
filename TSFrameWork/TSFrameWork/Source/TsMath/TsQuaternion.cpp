#include "TsQuaternion.h"
#include "TsVector.h"
#include "TsMatrix.h"
const TsQuaternion TsQuaternion::identity = TsQuaternion( 0 , 0 , 0 , 1 );

XMVECTOR TsQuaternion::ToXMVECTOR()const
{
    return XMLoadFloat4( this );
}

TsMatrix TsQuaternion::ToMatrix()const
{
    return TsMatrix::CreateRotate( *this );
}

TsVector3 TsQuaternion::ToEuler()const
{
    TsVector3 result;

    TsF32& yAxis = result.y;    //  y axis
    TsF32& zAxis= result.z;     //  z axis
    TsF32& xAxis = result.x;    //  x axis

    TsF32 sqx = x*x;
    TsF32 sqy = y*y;
    TsF32 sqz = z*z;
    TsF32 sqw = w*w;

    TsF32 unit = sqx + sqy + sqz + sqw;
    TsF32 test = x*y + z*w;
    if (test > 0.499f * unit)
    {
        // singularity at north pole
        yAxis = 2.0f * (TsF32)atan2f(x,w);
        zAxis = (TsF32)TS_PI / 2.0f;
        xAxis = 0.0f;
    } 
    else if (test < -0.499*unit)
    {
        // singularity at south pole
        yAxis = -2.0f * (TsF32)atan2f(x,w);
        zAxis = -(TsF32)TS_PI / 2.0f;
        xAxis = 0.0f;
    } 
    else
    {
        yAxis = (TsF32)atan2f((2.0f*y*w - 2.0f*x*z),
            (sqx - sqy - sqz + sqw));
        zAxis = (TsF32)asinf(2.0f*test / unit);
        xAxis = (TsF32)atan2f((2.0f*x*w - 2.0f*y*z),
            (-sqx + sqy - sqz + sqw));
    } 

    return TsDegree(result);
}


TsQuaternion  TsQuaternion::operator * ( const TsQuaternion& quaternion )const
{
    return TsQuaternion( XMQuaternionMultiply( ToXMVECTOR() , quaternion.ToXMVECTOR() ));
}

TsQuaternion& TsQuaternion::operator *=( const TsQuaternion& quaternion )
{
    return *this = *this * quaternion;
}

TsQuaternion TsQuaternion::Euler( const TsVector3& Euler )
{
    TsF32 x = Euler.x ;
    TsF32 y = Euler.y ;
    TsF32 z = Euler.z ;
    return 
        AngleAxis( TsVector3::right , TsRadian( x ) ) *
        AngleAxis( TsVector3::up , TsRadian( y ) ) *
        AngleAxis( TsVector3::front , TsRadian( z ) );
}

TsQuaternion TsQuaternion::AngleAxis( TsVector3 axis , FLOAT angle )
{
    return TsQuaternion( XMQuaternionRotationMatrix( XMMatrixRotationAxis( axis.ToXMVECTOR() , angle ) ) );
}