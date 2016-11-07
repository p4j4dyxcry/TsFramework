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
    TsMatrix m = ToMatrix();

    TsF32 r00 = m._11;
    TsF32 r01 = m._12;
    TsF32 r02 = m._13;

    TsF32 r10 = m._21;
    TsF32 r11 = m._22;
    TsF32 r12 = m._23;

    TsF32 r20 = m._31;
    TsF32 r21 = m._32;
    TsF32 r22 = m._33;

    TsVector3 result;
    TsF32& thetaX = result.x;
    TsF32& thetaY = result.y;
    TsF32& thetaZ = result.z;

    if( r02 < +1 )
    {
        if( r02 > -1 )
        {
            thetaY = asinf( r02 );
            thetaX = atan2f( -r12 , r22 );
            thetaZ = atan2f( -r01 , r00 );
        }
        else     // r02 = -1
        {
            // Not a unique solution: thetaZ - thetaX = atan2f(r10,r11)
            thetaY = -TS_PI / 2;
            thetaX = -atan2f( r10 , r11 );
            thetaZ = 0;
        }
    }
    else // r02 = +1
    {
        // Not a unique solution: thetaZ + thetaX = atan2f(r10,r11)
        thetaY = +TS_PI / 2;
        thetaX = atan2f( r10 , r11 );
        thetaZ = 0;
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