//!*******************************************************
//! TsMathUtility.h
//!
//! 数学関係の簡易計算ヘルパー
//!
//! © 2016 Yuki Tsuneyama
#pragma once

//----------------------------------------------------------
//! Define
//----------------------------------------------------------
class TsVector2;
class TsVector3;
class TsVector4;
class TsQuaternion;
class TsMatrix;
class TsTransForm;
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

//値を0 ~ 1に飽和させる
inline TsF32 TsSaturate(TsF32 f)
{
    return TsClamp(f, 0.0f, 1.0f);
}

inline TsVector2 TsSaturate(TsVector2 v)
{
    return TsClamp(v, TsVector2::zero, TsVector2::one);
}

inline TsVector3 TsSaturate(TsVector3 v)
{
    return TsClamp(v, TsVector3::zero, TsVector3::one);
}

inline TsVector4 TsSaturate(TsVector4 v)
{
    return TsClamp(v, TsVector4::zero, TsVector4::one);
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

//! Min Vector2 の特殊化
template<>
inline TsVector2 TsMin( const TsVector2& a , const TsVector2& b )
{
    return TsVector2( TsMin( a.x , b.x ) ,
                      TsMin( a.y , b.y ));
}

//! Min Vector3 の特殊化
template<>
inline TsVector3 TsMin( const TsVector3& a , const TsVector3& b )
{
    return TsVector3( TsMin( a.x , b.x ) ,
                      TsMin( a.y , b.y ) ,
                      TsMin( a.z , b.z ) );
}                                        

//! Min Vector4 の特殊化
template<>
inline TsVector4 TsMin( const TsVector4& a , const TsVector4& b )
{
    return TsVector4( TsMin( a.x , b.x ) ,
                      TsMin( a.y , b.y ) ,
                      TsMin( a.z , b.z ) ,
                      TsMin( a.w , b.w ) );
}

//! Max Vector2 の特殊化
template<>
inline TsVector2 TsMax( const TsVector2& a , const TsVector2& b )
{
    return TsVector2( TsMax( a.x , b.x ) ,
                      TsMax( a.y , b.y ) );
}

//! Max Vector3 の特殊化
template<>
inline TsVector3 TsMax( const TsVector3& a , const TsVector3& b )
{
    return TsVector3( TsMax( a.x , b.x ) ,
                      TsMax( a.y , b.y ) ,
                      TsMax( a.z , b.z ) );
}

//! Max Vector4 の特殊化
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

//! Quaternionの線形補完の特殊化
template<>
inline TsQuaternion TsLerp(const TsQuaternion& a, const TsQuaternion& b, TsF32 t)
{
    //! オイラー角に戻してオイラー角を線形補完する。
    TsVector3 euler = TsLerp(a.ToEuler(), b.ToEuler(), t);
    return TsQuaternion::CreateByEuler(euler);
}

// Transformの線形補完の特殊化

template<>
inline TsTransForm TsLerp(const TsTransForm& a, const TsTransForm& b, TsF32 t)
{
    TsTransForm result;

    result.m_localPosition = TsLerp(a.m_localPosition, b.m_localPosition, t);
    result.m_localScale = TsLerp(a.m_localScale, b.m_localScale, t);
    result.m_localRotate = TsLerp(a.m_localRotate,b.m_localRotate,t);

    return result;
}

//! 2次補完 ease - in
template<typename T>
inline T TsEaseIn(const T& a, const T&b, TsF32 t)
{
    return TsLerp(a, b, t*(2 - t));
}

//! 3次補完 ease - out
template<typename T>
inline T TsEaseOut(const T& a, const T&b, TsF32 t)
{
    return TsLerp(a, b, t * t);
}

//! a と b を t で3次補完する
template<typename T>
inline T TsQubic( const T& a , const T&b , TsF32 t )
{
    return TsLerp( a , b , t * t * ( 3.0f - 2.0f * t ) );
}

//絶対値計算
template<typename T>
inline T TsAbs(const T& t)
{
    return fabsf(t);
};

//絶対値計算のVector2特殊化
template<>
inline TsVector2 TsAbs(const TsVector2& v)
{
    return TsVector2(fabsf(v.x), fabs(v.y));
}

//絶対値計算のVector3特殊化
template<>
inline TsVector3 TsAbs(const TsVector3& v)
{
    return TsVector3(fabsf(v.x), fabs(v.y),fabsf(v.z));
}

//絶対値計算のVector4特殊化
template<>
inline TsVector4 TsAbs(const TsVector4& v)
{
    return TsVector4(fabsf(v.x), fabs(v.y),fabsf(v.z),fabsf(v.w));
}


//絶対値計算のQuaternion特殊化
template<>
inline TsQuaternion TsAbs(const TsQuaternion& v)
{
    return TsQuaternion(fabsf(v.x), fabs(v.y), fabsf(v.z), fabsf(v.w));
}

//! 三角形の法線を計算
inline TsVector3 TsComputeFaceNormal(const TsVector3& v0 ,const TsVector3& v1 , const TsVector3& v2 )
{
    TsVector3 n = TsVector3::Cross( ( v0 - v1 ) , ( v1 - v2 ) );
    return n.Normalized();
}
//! 三角形の角度（傾き)を計算
inline TsF32 TsComputeFaceAngle(const TsVector3& v0 ,const TsVector3& v1 ,const TsVector3& v2 )
{
    TsF32 angle = TsVector3::Dot( ( v0 - v1 ) , ( v1 - v2 ) );
    return acos( angle );
}
//! 三角形の面積を計算
inline TsF32 TsComputeTriangleArea(const TsVector3& v0 ,const TsVector3& v1 ,const TsVector3& v2 )
{
    TsVector3 cross = TsVector3::Cross( ( v0 - v1 ) , ( v1 - v2 ) );
    
    return cross.Length() / 2.0f;
}

//! ２つのベクトルの成すCosΘを計算する
inline TsF32 TsComputeCos(const TsVector2& v0,
                          const TsVector2& v1,
                          TsBool useNormalization = TS_TRUE)
{
    if (useNormalization)
        return TsVector2::Dot(v0.Normalized(), v1.Normalized());
    else
        return TsVector2::Dot(v0, v1);
}

//! ２つのベクトルの成すCosΘを計算する
inline TsF32 TsComputeCos(const TsVector3& v0,
                          const TsVector3& v1,
                          TsBool useNormalization = TS_TRUE)
{
    if (useNormalization)
        return TsVector3::Dot(v0.Normalized(), v1.Normalized());
    else
        return TsVector3::Dot(v0, v1);
}

//! ２つのベクトルの成すSinΘを計算する
inline TsF32 TsVectorComputeSin( const TsVector2& v0, 
                                 const TsVector2& v1,
                                 TsBool useNormalization = TS_TRUE)
{
    //! 外積からSinを求める
    if (useNormalization)
    {        
        TsF32 v0Len = v0.Length();
        TsF32 v1Len = v1.Length();

        return TsVector2::Cross(v0, v1).Length() / (v0Len * v1Len);
    }
    else
    {
        return TsVector2::Dot(v0, v1);
    }

}

//! ２つのベクトルの成すSinΘを計算する
inline TsF32 TsComputeSin(const TsVector3& v0,
                          const TsVector3& v1,
                          TsBool useNormalization = TS_TRUE)
{
    //! 外積からSinを求める
    if (useNormalization)
    {       
        TsF32 v0Len = v0.Length();
        TsF32 v1Len = v1.Length();

        return TsVector3::Cross(v0, v1).Length() / (v0Len * v1Len);
    }
    else
    {
        return TsVector3::Cross(v0, v1).Length();
    }

}

//! ガウス関数
inline TsF32 Gaussian(TsF32 x, TsF32 mean, TsF32 deviation)
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

