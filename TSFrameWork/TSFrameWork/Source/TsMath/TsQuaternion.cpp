#include "TsQuaternion.h"
#include "TsVector.h"
#include "TsMatrix.h"
const TsQuaternion TsQuaternion::identity = TsQuaternion( 0 , 0 , 0 , 1 );

//===================================================================
//  ! simdに変換
//===================================================================
XMVECTOR TsQuaternion::ToXMVECTOR()const
{
    return XMLoadFloat4( this );
}

//===================================================================
//  ! RotateMatrixに変換
//===================================================================
TsMatrix TsQuaternion::ToMatrix()const
{
    return TsMatrix::CreateRotate( *this );
}

//===================================================================
//  ! オイラー角に変換
//===================================================================
TsVector3 TsQuaternion::ToEuler()const
{
    TsVector3 result;

    TsF32& yAxis = result.y;    //  y axis
    TsF32& zAxis = result.z;    //  z axis
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

    return TsDegree( result );
}

//===================================================================
//  ! Euler →　Quaternion
//===================================================================
const TsQuaternion& TsQuaternion::Euler(TsF32 x, TsF32 y, TsF32 z)
{
    return *this = CreateByEuler( x , y, z);
}
//===================================================================
//  ! Euler →　Quaternion
//===================================================================
const TsQuaternion& TsQuaternion::Euler(const TsVector3& v)
{
    return *this = CreateByEuler(v.x, v.y, v.z);
}

//===================================================================
//  ! AngleAxis →　Quaternion
//===================================================================
const TsQuaternion& TsQuaternion::AngleAxis(const TsVector3& axis, TsF32 angle)
{
    return *this = CreateByAngleAxis(axis, angle);
}


//===================================================================
//  ! Quaternion * Quaternion
//===================================================================
TsQuaternion  TsQuaternion::operator * ( const TsQuaternion& quaternion )const
{
    return TsQuaternion( XMQuaternionMultiply( ToXMVECTOR() , quaternion.ToXMVECTOR() ));
}

//===================================================================
//  ! Quaternion *= Quaternion
//===================================================================
TsQuaternion& TsQuaternion::operator *=( const TsQuaternion& quaternion )
{
    return *this = *this * quaternion;
}

//===================================================================
//  ! Create Quaternion By Euler Angles
//===================================================================
TsQuaternion TsQuaternion::CreateByEuler(TsF32 x, TsF32 y, TsF32 z)
{
    return
        CreateByAngleAxis(TsVector3::right, TsRadian(x)) *
        CreateByAngleAxis(TsVector3::up, TsRadian(y)) *
        CreateByAngleAxis(TsVector3::front, TsRadian(z));
}

//===================================================================
//  ! Create Quaternion By Euler Angles
//===================================================================
TsQuaternion TsQuaternion::CreateByEuler( const TsVector3& Euler )
{
    TsF32 x = Euler.x ;
    TsF32 y = Euler.y ;
    TsF32 z = Euler.z ;
    return CreateByEuler(x,y,z);
}

//===================================================================
//  ! Create Quaternion By Axis Angle
//===================================================================
TsQuaternion TsQuaternion::CreateByAngleAxis(const TsVector3& axis , TsF32 angle )
{
    return TsQuaternion( XMQuaternionRotationMatrix( XMMatrixRotationAxis( axis.ToXMVECTOR() , angle ) ) );
}