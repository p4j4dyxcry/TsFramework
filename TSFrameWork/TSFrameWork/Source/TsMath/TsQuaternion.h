//!*******************************************************
//!　TsQuaternion.h
//!
//!　クォータニオン
//!
//! © 2016 Yuki Tsuneyama
#pragma once

#include "../../Extern/xna/Xna.h"

class TsQuaternion;
class TsMatrix;
class TsVector3;
class TsVector4;

//!*******************************************************
//!　[クラス] TsQuaternion
class TsQuaternion : public XMFLOAT4
{
public:
    using oprator = XMFLOAT4;
    TsQuaternion( FLOAT _x = 0 , FLOAT _y = 0 , FLOAT _z = 0 , FLOAT _w = 1 ) :XMFLOAT4( _x , _y , _z , _w ){};
    TsQuaternion( XMFLOAT4 vector4 ) : XMFLOAT4( vector4.x , vector4.y , vector4.z , vector4.w ){};
    TsQuaternion( XMVECTOR vector )	: XMFLOAT4( vector.m128_f32[0] , vector.m128_f32[1] , vector.m128_f32[2] , vector.m128_f32[3] ){};
    XMVECTOR  ToXMVECTOR()const;
    TsMatrix  ToMatrix()const;

    TsQuaternion& operator = ( TsMatrix matrix );
    TsQuaternion  operator * ( const TsQuaternion& quaternion )const;
    TsQuaternion& operator *=( const TsQuaternion& quaternion );

    static TsQuaternion AngleAxis( TsVector3 axis , FLOAT angle );
    static const TsQuaternion identity;
protected:

};