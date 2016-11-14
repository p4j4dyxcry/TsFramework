#include "../../TsAfx.h"

//----------------------------------------------------------
//! template関数明示的のインスタンス化
//----------------------------------------------------------
template TsBool CollisionPointAndPoint(const TsVector3&, const TsVector3&, TsF32);
template TsBool CollisionPointAndPoint(const TsVector2&, const TsVector2&, TsF32);
template TsBool CollisionLineAndPoint(const TsLine2D&, const TsVector2&, TsF32);
template TsBool CollisionLineAndPoint(const TsLine3D&, const TsVector3&, TsF32);
template TsBool CollisionLineAndLine(const TsLine2D&, const TsLine2D&, TsF32, TsVector2*);
template TsBool CollisionLineAndLine(const TsLine3D&, const TsLine3D&, TsF32, TsVector3*);
template TsBool CollisionSphereAndRay(const TsCircle&, const TsLine2D&, TsF32, TsVector2*, TsVector2*);
template TsBool CollisionSphereAndRay(const TsSphere3D&, const TsLine3D&, TsF32, TsVector3*, TsVector3*);
template TsBool CollisionSphereAndLine(const TsCircle&, const TsLine2D&, TsF32);
template TsBool CollisionSphereAndLine(const TsSphere3D&, const TsLine3D&, TsF32);


//----------------------------------------------------------
//! 点と点
//  @ref 衝突点は点と等しい
//----------------------------------------------------------
template< typename T>
inline TsBool CollisionPointAndPoint(const T& p0,   //点1
                                     const T& p1,   //点2
                                     //誤差許容範囲
                                     TsF32 tolerance)
{
    //２つ点の位置がほぼ等しいかどうか
    return fabs((p0 - p1).Length()) < tolerance;
}

//----------------------------------------------------------
//! 線分と点
//  @ref 衝突点は点1と等しい
//----------------------------------------------------------
template< typename T>
inline TsBool CollisionLineAndPoint(const TsLine<T>& line,  //線分1
                                    const T& point ,        //点1
                                    // 誤差許容範囲
                                    TsF32 tolerance)
{
    T pointToLineBegin = point - line.GetBegin();
    T lineVector = line.GetVector();

    // ２つのベクトルは平行？
    TsBool isParallel = fabsf(T::Cross(pointToLineBegin, lineVector).Length()) < tolerance;

    //ベクトルの向きは反対を向いていない？
    TsBool isSigne = T::Dot(pointToLineBegin, lineVector) >= 0;

    //点から線の視点までの長さが終点を超えていない？
    TsF32 length1 = pointToLineBegin.Length();
    TsF32 length2 = lineVector.Length();

    TsBool isInLength = length1 < length2;

    return isParallel & isSigne;
}

//----------------------------------------------------------
//! 線分と線分　の衝突判定
//  @param  line0       線分1
//  @param  line1       線分2
//  @param  tolerance   誤差許容範囲  (optin)
//  @param  pOut        衝突点を求める(option)
//----------------------------------------------------------
template< typename T>
inline TsBool CollisionLineAndLine( const TsLine<T> &line0, // 線分1
                                    const TsLine<T> &line1, // 線分2                                    
                                    TsF32 tolerance,        //誤差許容範囲
                                    T*     pOut)            // 交点(出力)
{

    T v = line0.GetBegin() - line1.GetBegin();
    T Crs_v1_v2 = T::Cross(line0.GetVector(), line1.GetVector());

    // ２つのベクトルは平行？
    if (fabsf(Crs_v1_v2.Length()) < tolerance)
        return TS_FALSE;

    // ベクトルの外積からsinを取得
    TsF32 t1 = TsComputeSin(v, line0.GetEnd());
    TsF32 t2 = TsComputeSin(v, line1.GetEnd());

    if (t1 + tolerance < 0 ||
        t1 - tolerance > 1 ||
        t2 + tolerance < 0 ||
        t2 - tolerance > 1)
    {
        // 交差していない
        return TS_FALSE;
    }

    if (pOut != nullptr)
        *pOut = line0.GetBegin() + line0.GetEnd() * t1;

    return TS_TRUE;
}

//----------------------------------------------------------
//! 円と点
//----------------------------------------------------------
inline TsBool CollisionCircleAndPoint(const TsCircle& circle,
                                      const TsVector2& point)
{
    TsVector2&& center = circle.GetCenter();
    TsF32     r      = circle.GetRadius();

    TsF32 x  = (point.x - center.x) * 
               (point.x - center.x);

    TsF32 y  = (point.y - center.y) * 
               (point.y - center.y);

    TsF32 r2 = r * r;

    return x + y <= r2;
}

//----------------------------------------------------------
//! 円と円
//----------------------------------------------------------
inline TsBool CollisionCircleAndCircle(const TsCircle& c0,
                                       const TsCircle& c1)
{
    TsVector2&& center0 = c0.GetCenter();
    TsVector2&& center1 = c1.GetCenter();

    TsF32 r0 = c0.GetRadius();
    TsF32 r1 = c1.GetRadius();

    TsF32 x = (center0.x - center1.x) *
              (center0.x - center1.x);

    TsF32 y = (center0.y - center1.y) *
              (center0.y - center1.y);

    TsF32 r2 = (r0 + r1) * (r0 + r1);

    return x + y <= r2;
}

//----------------------------------------------------------
//! 球または円と線分の衝突判定
//  @param  sphere      球または円
//  @param  ray         レイ(無限に続く線)
//  @param  tolerance   誤差許容範囲  (optin)
//  @param  pOut1       近景の衝突点を求める(option)
//  @param  pOut2       遠景の衝突点を求める(option)
//----------------------------------------------------------
template< typename T>
inline TsBool CollisionSphereAndRay(const TsSphere<T>& sphere,
                                     const TsLine<T>& line,
                                     //誤差許容範囲
                                     TsF32 tolerance ,
                                     T* pOut0,
                                     T* pOut1)
{
    T&& offset = line.GetBegin() - sphere.GetCenter();
    T&& vec    = line.GetNormalizeVector();
    TsF32 radius = sphere.GetRadius();

    // 係数tを算出
    TsF32 cos01 = T::Dot(offset, vec);
    TsF32 cos02 = T::Dot(offset, offset);

    TsF32 s = cos01 * cos01 - cos02 + radius * radius;

    // 誤差を修正
    if (TsAbs(s) < tolerance)
        s = 0.0f;
    else if (s < 0.0f)
        return TS_FALSE;

    TsF32 sq = sqrtf(s);
    TsF32 t0 = -cos01 - sq;
    TsF32 t1 = -cos01 + sq;

    if (t0 < 0 || t1 < 0)
        return TS_FALSE;

    // 衝突座標を出力
    if (pOut0)
        *pOut0 = offset + sphere.GetCenter() + vec * t0;
    if (pOut1)
        *pOut1 = offset + sphere.GetCenter() + vec * t1;
    
    return TS_TRUE;
}

//----------------------------------------------------------
//! 球と線分の衝突判定
//  @param  circle      円
//  @param  ray         レイ(無限に続く線)
//  @param  tolerance   誤差許容範囲  (optin)
//  @param  pOut1       近景の衝突点を求める(option)
//  @param  pOut2       遠景の衝突点を求める(option)
//----------------------------------------------------------

TsBool CollisionCircleAndRay( const TsCircle& circle,
                              const TsLine2D& line,
                              //誤差許容範囲
                              TsF32 tolerance ,
                              TsVector2* pOut0,
                              TsVector2* pOut1)
{
    return CollisionSphereAndRay(circle,line,tolerance,pOut0,pOut1);
}

//----------------------------------------------------------
//! 球と点
//  @return 衝突 true 
//----------------------------------------------------------
inline TsBool CollisionSphereAndPoint( const TsSphere3D& s0,
                                       const TsVector3& p0)
{
    TsVector3&& c0 = s0.GetCenter();
    TsF32       r = s0.GetRadius();

    TsF32 x = (p0.x - c0.x) *
              (p0.x - c0.x);

    TsF32 y = (p0.y - c0.y) *
              (p0.y - c0.y);

    TsF32 z = (p0.z - c0.z) *
              (p0.z - c0.z);

    TsF32 r2 = r * r;

    return x + y + z <= r2;
}

//----------------------------------------------------------
//! 球と球
//----------------------------------------------------------
inline TsBool CollisionSphereAndSphere(const TsSphere3D& s0,
                                       const TsSphere3D& s1)
{
    TsVector3&& c0 = s0.GetCenter();
    TsVector3&& c1 = s1.GetCenter();
    
    TsF32       r0 = s0.GetRadius();
    TsF32       r1 = s1.GetRadius();

    TsF32 x = (c0.x - c1.x) *
              (c0.x - c1.x);

    TsF32 y = (c0.y - c1.y) *
              (c0.y - c1.y);

    TsF32 z = (c0.z - c1.z) *
              (c0.z - c1.z);

    TsF32 r2 = (r0 + r1) * (r0 + r1);

    return x + y + z <= r2;
}


//----------------------------------------------------------
//! 球または円と線分の衝突判定
//  @param  s           円か球
//  @param  line        線分
//  @param  tolerance   誤差許容範囲  (optin)
//----------------------------------------------------------
template<typename T>
inline TsBool CollisionSphereAndLine( const TsSphere<T>& s,
                                      const TsLine<T>& line,
                                     //誤差許容範囲
                                     TsF32 tolerance)
{

    T&& v0 = s.GetCenter() - line.GetBegin();
    T&& v1 = line.GetVector();
    TsF32 r = s.GetRadius();

    TsF32 d = T::Dot(v0, v1);

    if (d < 0)
    {
        return v0.Length() < r;
    }

    TsF32 d2 = T::Dot(v1, v1);
    TsF32 r2 = r*r;
    if (d > d2)
    {
        T&& v2 = s.GetCenter() - line.GetEnd();
        TsF32 len = v2.Length();
        return (len * len) < r2;
    }
    else
    {
        TsF32 d3 = T::Dot( v0, v0 );
        return d3 - (d / d2) * d < r2;
    }
}

//----------------------------------------------------------
//! 円と線分の衝突判定
//  @param  circle      円
//  @param  line        線分
//  @param  tolerance   誤差許容範囲  (optin)
//----------------------------------------------------------
inline TsBool CollisionCircleAndLine(const TsCircle& circle,
                                     const TsLine2D& line,
                                     //誤差許容範囲
                                     TsF32 tolerance)
{
    return CollisionSphereAndLine(circle, line, tolerance);
}
