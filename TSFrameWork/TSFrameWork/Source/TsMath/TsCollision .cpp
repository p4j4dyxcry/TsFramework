#include "../../TsAfx.h"

//----------------------------------------------------------
//! template関数明示的のインスタンス化
//----------------------------------------------------------
template TsBool CollisionPointAndPoint(const TsVector3&, const TsVector3&, TsF32);
template TsBool CollisionPointAndPoint(const TsVector2&, const TsVector2&, TsF32);
template TsBool CollisionLineAndPoint(const TsLine2D&, const TsVector2&, TsF32);
template TsBool CollisionLineAndPoint(const TsLine3D&, const TsVector3&, TsF32);
template TsBool CollisionRayAndPoint(const TsLine2D&, const TsVector2&, TsF32);
template TsBool CollisionRayAndPoint(const TsLine3D&, const TsVector3&, TsF32);
template TsBool CollisionLineAndLine(const TsLine2D&, const TsLine2D&, TsF32, TsVector2*);
template TsBool CollisionLineAndLine(const TsLine3D&, const TsLine3D&, TsF32, TsVector3*);
template TsBool CollisionSphereAndRay(const TsCircle&, const TsLine2D&, TsF32, TsVector2*, TsVector2*);
template TsBool CollisionSphereAndRay(const TsSphere3D&, const TsLine3D&, TsF32, TsVector3*, TsVector3*);
template TsBool CollisionSphereAndLine(const TsCircle&, const TsLine2D&, TsF32);
template TsBool CollisionSphereAndLine(const TsSphere3D&, const TsLine3D&, TsF32);

template TsBool CollisionSphereAndSphere(const TsSphere2D&,const TsSphere2D&);
template TsBool CollisionSphereAndSphere(const TsSphere3D&, const TsSphere3D&);

template TsBool CollisionSphereAndPoint(const TsCircle& , const TsVector2& point);
template TsBool CollisionSphereAndPoint(const TsSphere3D& , const TsVector3& point);

template TsBool CollisionRayAndPlane(const TsVector2&, const TsLine2D&, TsF32);
template TsBool CollisionRayAndPlane(const TsVector3&, const TsLine3D&, TsF32);

template TsBool CollisionAABBAndRay(const TsAABB2D&, const TsLine2D&, TsF32,TsVector2*);
template TsBool CollisionAABBAndRay(const TsAABB3D&, const TsLine3D&, TsF32,TsVector3*);

template TsBool CollisionAABBAndSphere(const TsAABB2D& , const TsSphere2D& );
template TsBool CollisionAABBAndSphere(const TsAABB3D& , const TsSphere3D& );

template TsBool CollisionAABBAndAABB(const TsAABB2D&, const TsAABB2D&);
template TsBool CollisionAABBAndAABB(const TsAABB3D&, const TsAABB3D&);

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
//! レイと点
//  @ref 衝突点は点1と等しい
//----------------------------------------------------------
template< typename T>
inline TsBool CollisionRayAndPoint( const TsLine<T>& ray,  //レイ
                                    const T& point ,        //点1
                                    // 誤差許容範囲
                                    TsF32 tolerance)
{
    T pointToLineBegin = point - ray.GetBegin();
    T lineVector = ray.GetVector();

    // ２つのベクトルは平行？
    return fabsf(T::Cross(pointToLineBegin, lineVector).Length()) < tolerance;
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
//! 円or球と点
//  @return 衝突 true 
//----------------------------------------------------------
template< typename T>
TsBool CollisionSphereAndPoint(const TsSphere<T>& sphere,
                               const T& point)
{
    T&& center = sphere.GetCenter();
    TsF32 r = sphere.GetRadius();
    TsF32 value = 0;
    TsInt sz = sizeof(T) / sizeof(TsF32);
    for (TsInt i = 0; i < sz; ++i)
        value += (point[i] - center[i]) * (point[i] - center[i]);

    TsF32 r2 = r*r;

    return value <= r2;
}

//----------------------------------------------------------
//! 円と点
//----------------------------------------------------------
inline TsBool CollisionSphere2DAndPoint(const TsCircle& circle,
                                        const TsVector2& point)
{
    return CollisionSphereAndPoint(circle,point);
}

//----------------------------------------------------------
//! 円と円
//----------------------------------------------------------
inline TsBool CollisionSphereAndSphere2D(const TsCircle& c0,
                                       const TsCircle& c1)
{
    return CollisionSphereAndSphere(c0,c1);
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

TsBool CollisionSphereAndRay2D( const TsCircle& circle,
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
inline TsBool CollisionSphereAndPoint3D( const TsSphere3D& s0,
                                         const TsVector3& p0)
{
    return CollisionSphereAndPoint(s0, p0);
}

//----------------------------------------------------------
//! 円と円 or 球と球
//  @return 衝突 true 
//----------------------------------------------------------
template< typename T>
inline TsBool CollisionSphereAndSphere  (const TsSphere<T>& s0,
                                         const TsSphere<T>& s1)
{
    T&& c0 = s0.GetCenter();
    T&& c1 = s1.GetCenter();

    TsInt sz = sizeof(T) / sizeof(TsF32);

    TsF32       r0 = s0.GetRadius();
    TsF32       r1 = s1.GetRadius();

    TsF32 value = 0;
    for (TsInt i = 0; i < sz; ++i)
        value += (c0[i] - c1[i]) * (c0[i] - c1[i]);

    TsF32 r2 = (r0 + r1) * (r0 + r1);

    return value <= r2;
}

//----------------------------------------------------------
//! 球と球
//----------------------------------------------------------
inline TsBool CollisionSphereAndSphere3D(const TsSphere3D& s0,
                                       const TsSphere3D& s1)
{
    return CollisionSphereAndSphere(s0, s1);
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
inline TsBool CollisionSphereAndLine2D(const TsCircle& circle,
                                       const TsLine2D& line,
                                     //誤差許容範囲
                                     TsF32 tolerance)
{
    return CollisionSphereAndLine(circle, line, tolerance);
}


//----------------------------------------------------------
//! 平面とレイの衝突判定
//  @param  normal      面のベクトル
//  @param  ray         レイ
//  @param  tolerance   誤差許容範囲  (optin)
//----------------------------------------------------------
template<typename T>
inline TsBool CollisionRayAndPlane(const T& normal,
                                  const TsLine<T>& ray,
                                  //誤差許容範囲
                                  TsF32 tolerance )
{
    return T::Dot(normal, ray.GetVector()) != 0;
}


//----------------------------------------------------------
//! 線分と三角形
//  @param  p0          頂点
//  @param  p1          頂点
//  @param  p2          頂点
//  @param  line        線分
//  @param  tolerance   誤差許容範囲  (optin)
//  @param  pOut        交差点
//----------------------------------------------------------
TsBool CollisionLineAndTriangle(const TsVector3& p0,
                                const TsVector3& p1,
                                const TsVector3& p2,
                                const TsLine3D& line,
                                //誤差許容範囲
                                TsF32 tolerance ,
                                TsVector3* pOut )

{
    TsVector3 v0 = (p1 - p0).Normalized();
    TsVector3 v1 = (p2 - p0).Normalized();

    TsVector3 lineVector = line.GetNormalizeVector();

    TsVector3 normal = TsVector3::Cross(v1, v0);

    TsVector3 x = p0 - lineVector;

    TsF32 dot0 = TsVector3::Dot(x, normal);
    TsF32 dot1 = TsVector3::Dot(lineVector, normal);

    TsF32 t = dot0 / dot1;

    if (t < 0)
        return TS_FALSE;
    if (t > line.GetVector().Length())
        return TS_FALSE;

    TsVector3 p = line.GetBegin() + (lineVector * t);

    TsVector3 d0, d1, cross;

    // 1つ目の辺の中に納まっているか判定
    {
        d0 = p - p0;
        d1 = p1 - p0;

        cross = TsVector3::Cross(d0, d1);
        if (TsVector3::Dot(cross, normal) < 0)
            return TS_FALSE;
    }

    // 2つ目の辺の中に納まっているか判定
    {
        d0 = p - p1;
        d1 = p2 - p1;

        cross = TsVector3::Cross(d0, d1);
        if (TsVector3::Dot(cross, normal) < 0)
            return TS_FALSE;
    }

    // 3つ目の辺の中に納まっているか判定
    {
        d0 = p - p2;
        d1 = p0 - p2;

        cross = TsVector3::Cross(d0, d1);
        if (TsVector3::Dot(cross, normal) < 0)
            return TS_FALSE;
    }

    return TS_TRUE;
}

//----------------------------------------------------------
//! AABB と　AABB
//  @param  aabb0          AABB
//  @param  aabb1          AABB
//----------------------------------------------------------
template<typename T>
TsBool CollisionAABBAndAABB(const TsAABB<T>& a,
                            const TsAABB<T>& b)
{
    const T& aMin = a.GetMin();
    const T& aMax = a.GetMax();
    const T& bMin = b.GetMin();
    const T& bMax = b.GetMax();

    // 次元数を計算
    TsInt sz = sizeof(T) / sizeof(TsF32);

    for (TsInt i = 0; i < sz; ++i)
    {
        //! 次元が衝突しているか判定
        //　次元は x -> y -> z -> w ...etc　と∞次元まで判定できる。
        if ( aMin[i] > bMax[i] || bMin[i] > aMax[i] )
            return TS_FALSE;
    }
    return TS_TRUE;
}

//=====================================================
// * 【↑】一つの関数で計算できるようにした
//=====================================================
//template<> //2d
//TsBool CollisionAABBAndAABB( const TsAABB2D& a,
//                             const TsAABB2D& b)
//{
//    //! メモ 2dと3dを一つの関数で行えるようにする？
//    //　現状は
//    const TsVector2& aMin = a.GetMin();
//    const TsVector2& aMax = a.GetMax();
//    const TsVector2& bMin = b.GetMin();
//    const TsVector2& bMax = b.GetMax();
//    return aMin.x < bMax.x && bMin.x < aMax.x
//        && aMin.y < bMax.y && bMin.y < aMax.y;
//        
//}
//template<> //3d
//TsBool CollisionAABBAndAABB( const TsAABB3D& a,
//                             const TsAABB3D& b)
//{
//    const TsVector3& aMin = a.GetMin();
//    const TsVector3& aMax = a.GetMax();
//    const TsVector3& bMin = b.GetMin();
//    const TsVector3& bMax = b.GetMax();
//    return aMin.x < bMax.x && bMin.x < aMax.x
//        && aMin.y < bMax.y && bMin.y < aMax.y
//        && aMin.z < bMax.z && bMin.z < aMax.z;
//}


//----------------------------------------------------------
//! AABB と　Ray
//  @param  aabb           AABB
//  @param  rayDir         レイ
//  @prama  tolerance      誤差許容範囲
//  @param  pOut           衝突点(option)
//----------------------------------------------------------
template<typename T>
TsBool CollisionAABBAndRay (const TsAABB<T>& aabb,
                            const TsLine<T>& ray,
                            TsF32 tolerance,
                            T * pOut )
{
    // ベクトルの次元に合わせた要素数の取得
    // Vector2 -> 2
    // Vector3 -> 3
    TsInt sz = sizeof(T) / sizeof(TsF32);

    const T& rayPos = ray.GetBegin();
    const T& rayDir = ray.GetNormalizeVector();
    const T& min = aabb.GetMin();
    const T& max = aabb.GetMax();

    TsF32 t = -FLT_MAX;
    TsF32 t_max = FLT_MAX;

    //! 交差判定
    for (int i = 0; i < sz; ++i)
    {
        if ( fabsf(rayDir[i]) < tolerance)
        {
            if (rayPos[i] < min[i] || rayPos[i] >max[i])
                return TS_FALSE; // 交差していない
        }
        else 
        {
            // スラブとの距離を算出
            // t1が近スラブ、t2が遠スラブとの距離
            TsF32 odd = 1.0f / rayDir[i];
            TsF32 t1 = (min[i] - rayPos[i]) * odd;
            TsF32 t2 = (max[i] - rayPos[i]) * odd;
            if (t1 > t2) 
            {
                // swap
                TsF32 tmp = t1; t1 = t2; t2 = tmp;
            }

            if (t1 > t)
                t = t1;
            if (t2 < t_max)
                t_max = t2;

            // スラブ交差チェック
            if (t >= t_max)
                return TS_FALSE;
        }
    }

    // 衝突点を求める
    if (pOut) 
    {
        *pOut = rayPos + t * rayDir;
    }

    return TS_TRUE;
}

//----------------------------------------------------------
//! AABB　と 線分
//  @param  aabb           AABB
//  @param  line           線分
//  @prama  tolerance      誤差許容範囲
//  @param  pOut           衝突点(option)
//----------------------------------------------------------
template<typename T>
TsBool CollisionAABBAndLine(const TsAABB<T>& aabb,
                            const TsSphere<T>& Line,
                            TsF32 tolerance ,
                            T * pOut)
{
    //衝突点t
    T t;
    // レイと線分のあたり判定を求める
    TsBool bRayAndAABB = 
        CollisionAABBAndRay(aabb, 
                            Line, 
                            tolerance, 
                            &t);

    // レイが衝突していなかったら衝突しない
    if (bRayAndAABB == TS_FALSE)
        return TS_FALSE;

    // 衝突点と線分のあたり判定を求める
    TsBool isLineAndPoint =
        CollisionLineAndPoint(Line, t, tolerance);

    if ( isLineAndPoint && pOut )
        *pOut = t;

    return isLineAndPoint;
}

//----------------------------------------------------------
//! AABB　と 円or球
//----------------------------------------------------------
template<typename T>
TsBool CollisionAABBAndSphere(const TsAABB<T>& aabb,
                              const TsSphere<T>& sphere)
{
    TsVector<T>&& v = aabb.GetVertexList();

    for (auto it : v)
    {
        //全てのAABBの頂点と円or球のあたり判定を行う
        if ( CollisionSphereAndPoint(sphere, it) )
            return TS_TRUE;
    }
    return TS_FALSE;
}