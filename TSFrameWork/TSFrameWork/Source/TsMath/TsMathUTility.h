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
template<typename T>
inline T TsRadian( const T& degree )
{
    return degree * TS_PI / 180.0f;
}


//! radian -> degree
template<typename T>
inline T TsDegree( const T& radian )
{
    return radian * 180.0f / TS_PI;
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

class TsVector2;
class TsVector3;
class TsVector4;

template<>
inline TsVector2 TsMin( const TsVector2& a , const TsVector2& b )
{
    return TsVector2( TsMin( a.x , b.x ) ,
                      TsMin( a.y , b.y ));
}

template<>
inline TsVector3 TsMin( const TsVector3& a , const TsVector3& b )
{
    return TsVector3( TsMin( a.x , b.x ) ,
                      TsMin( a.y , b.y ) ,
                      TsMin( a.z , b.z ) );
}                                        

template<>
inline TsVector4 TsMin( const TsVector4& a , const TsVector4& b )
{
    return TsVector4( TsMin( a.x , b.x ) ,
                      TsMin( a.y , b.y ) ,
                      TsMin( a.z , b.z ) ,
                      TsMin( a.w , b.w ) );
}

template<>
inline TsVector2 TsMax( const TsVector2& a , const TsVector2& b )
{
    return TsVector2( TsMax( a.x , b.x ) ,
                      TsMax( a.y , b.y ) );
}

template<>
inline TsVector3 TsMax( const TsVector3& a , const TsVector3& b )
{
    return TsVector3( TsMax( a.x , b.x ) ,
                      TsMax( a.y , b.y ) ,
                      TsMax( a.z , b.z ) );
}

template<>
inline TsVector4 TsMax( const TsVector4& a , const TsVector4& b )
{
    return TsVector4( TsMax( a.x , b.x ) ,
                      TsMax( a.y , b.y ) ,
                      TsMax( a.z , b.z ) ,
                      TsMax( a.w , b.w ) );
}


//! a と b を t で線形補完する
template<typename T>
inline T TsLerp( const T& a , const T&b , TsF32 t )
{
    return a * ( 1.0f - t ) + b * t;
}
class TsQuaternion;
template<>
inline TsQuaternion TsLerp(const TsQuaternion& a, const TsQuaternion& b, TsF32 t)
{
    TsVector3 euler = TsLerp(a.ToEuler(), b.ToEuler(), t);
    return TsQuaternion::CreateByEuler(euler);
}
// 線形補完 の特殊化
class TsTransForm;
template<>
inline TsTransForm TsLerp(const TsTransForm& a, const TsTransForm& b, TsF32 t)
{
    TsTransForm result;

    result.m_localPosition = TsLerp(a.m_localPosition, b.m_localPosition, t);
    result.m_localScale = TsLerp(a.m_localScale, b.m_localScale, t);
    result.m_localRotate = TsLerp(a.m_localRotate,b.m_localRotate,t);

    return result;
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
    
    return cross.Length() / 2.0f;
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

    return 
        (1.0f / sqrt(2.0f * TS_PI * deviation * deviation))
        * expf((-((x - mean) * (x - mean))) / (2.0f * deviation * deviation));
}

