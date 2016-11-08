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

    //=======================================================
    //! Constructor
    //=======================================================
    TsQuaternion( FLOAT _x = 0 , FLOAT _y = 0 , FLOAT _z = 0 , FLOAT _w = 1 ) :XMFLOAT4( _x , _y , _z , _w ){};
    TsQuaternion( XMFLOAT4 vector4 ) : XMFLOAT4( vector4.x , vector4.y , vector4.z , vector4.w ){};
    TsQuaternion( XMVECTOR vector )	: XMFLOAT4( vector.m128_f32[0] , vector.m128_f32[1] , vector.m128_f32[2] , vector.m128_f32[3] ){};

    //=======================================================
    //! public method
    //=======================================================
    XMVECTOR  ToXMVECTOR()const;    // simdに変換
    TsMatrix  ToMatrix()const;      // Rotate Matrixに変換
    TsVector3 ToEuler()const;       // Euler角に変換

    const TsQuaternion& Euler(TsF32, TsF32, TsF32);
    const TsQuaternion& Euler(const TsVector3&);
    const TsQuaternion& AngleAxis(const TsVector3& axis, TsF32 angle);

    //=======================================================
    //! operator
    //=======================================================
    TsQuaternion& operator = ( TsMatrix matrix );
    TsQuaternion  operator * ( const TsQuaternion& quaternion )const;
    TsQuaternion& operator *=( const TsQuaternion& quaternion );

    //=======================================================
    //! static method
    //=======================================================
    static TsQuaternion CreateByEuler( const TsVector3& Euler );
    static TsQuaternion CreateByEuler(TsF32 x, TsF32 y, TsF32 z);
    static TsQuaternion CreateByAngleAxis(const TsVector3& axis, TsF32 angle);

    //=======================================================
    //! static member
    //=======================================================
    static const TsQuaternion identity;
protected:

};