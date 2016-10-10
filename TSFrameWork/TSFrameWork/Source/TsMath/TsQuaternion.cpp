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
  return XMQuaternionRotationRollPitchYaw( TsRadian( Euler.x ) ,
                                           TsRadian( Euler.y ) ,
                                           TsRadian( Euler.z ) );
}

TsQuaternion TsQuaternion::AngleAxis( TsVector3 axis , FLOAT angle )
{
    return TsQuaternion( XMQuaternionRotationMatrix( XMMatrixRotationAxis( axis.ToXMVECTOR() , angle ) ) );
}