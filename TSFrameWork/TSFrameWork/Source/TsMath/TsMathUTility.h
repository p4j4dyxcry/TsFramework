//!*******************************************************
//! TsMathUtility.h
//!
//! 数学関係の簡易計算ヘルパー
//!
//! © 2016 Yuki Tsuneyama
#pragma once

//! Pi
#define TS_PI (3.14159265359f)

//! degree -> radian
inline TsF32 TsRadian( TsF32 degree )
{
    return degree * TS_PI / 180.0f;;

}

//! radian -> degree
inline TsF32 TsDegree( TsF32 radian )
{
    return radian * 180 / TS_PI;;
}

//! 値を min ~ max　に設定
template<typename T>
inline T TsClamp( const T& value , const T& min , const T& max )
{
    return value < min ? min : value > max ? max : value;
}

//! a と bの大きい方を取得
template<typename T>
inline T TsMax( const T& a , const T& b )
{
    return a > b ? a : b;
}

//! a と bの小さいほうを取得
template<typename T>
inline T TsMin( const T& a , const T& b )
{
    return a < b ? a : b;
}

//! a と b を t で線形補完する
template<typename T>
inline T TsLerp( const T& a , const T&b , TsF32 t )
{
    return a * ( 1 - t ) + b * t;
}

//! a と b を t で3次補完する
template<typename T>
inline T TsQubic( const T& a , const T&b , TsF32 t )
{
    return TsLerp( a , b , t * t * ( 3.0f - 2.0f * t ) );
}
//! 三角形の法線を計算
inline TsVector3 TsComputeFaceNormal( TsVector3 v0 , TsVector3 v1 , TsVector3 v2 )
{
    TsVector3 n = TsVector3::Cross( ( v0 - v1 ) , ( v1 - v2 ) );
    return n.Normalized();
}
//! 三角形の角度（傾き)を計算
inline TsF32 TsComputeFaceAngle( TsVector3 v0 , TsVector3 v1 , TsVector3 v2 )
{
    TsF32 angle = TsVector3::Dot( ( v0 - v1 ) , ( v1 - v2 ) );
    return acos( angle );
}
//! 三角形の面積を計算
inline TsF32 TsComputeTriangleArea( TsVector3 v0 , TsVector3 v1 , TsVector3 v2 )
{
    TsVector3 cross = TsVector3::Cross( ( v0 - v1 ) , ( v1 - v2 ) );
    
    return cross.Length() / 2;
}

//! ガウス関数
inline TsF32 Gaussian(float x, float mean, float deviation)
{
    // The gaussian equation is defined as such:
    /*
    -(x - mean)^2
    -------------
    1.0               2*std_dev^2
    f(x,mean,std_dev) = -------------------- * e^
    sqrt(2*pi*std_dev^2)

    */

    return (1.0f / sqrt(2.0f * TS_PI * deviation * deviation))
        * expf((-((x - mean) * (x - mean))) / (2.0f * deviation * deviation));
}