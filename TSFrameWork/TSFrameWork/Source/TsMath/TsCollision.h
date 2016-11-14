//!*******************************************************
//! TsCollision.h
//!
//! コリジョン.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

//コリジョンの許容する誤差の規定値
static const TsF32 COLLISION_DEFAULT_TOLERANCE = 0.000001f;

//----------------------------------------------------------
//! 点と点
//----------------------------------------------------------
template< typename T>
inline TsBool CollisionPointAndPoint(const T& p0,   //点1
                                     const T& p1,   //点2
                                     //誤差許容範囲
                                     TsF32 tolerance = COLLISION_DEFAULT_TOLERANCE)
{
    //２つ点の位置がほぼ等しいかどうか
    return fabs((p0 - p1).Length()) < tolerance;
}

//----------------------------------------------------------
//! 線分と点
//----------------------------------------------------------
template< typename T>
inline TsBool CollisionLineAndPoint(const TsLine<T>& line,  //線分1
                                    const T& point ,        //線分2
                                    // 誤差許容範囲
                                    TsF32 tolerance = COLLISION_DEFAULT_TOLERANCE)
{
    T pointToLineBegin = point - line.GetBegin();
    T lineVector = line.GetVector();

    // ２つのベクトルは平行？
    TsBool isParallel fabsf(T::Cross(pointToLineBegin, lineVector).Length()) < tolerance;

    //ベクトルの向きは反対を向いていない？
    TsBool isSigne  T::Dot(pointToLineBegin, lineVector) >= 0;

    //点から線の視点までの長さが終点を超えていない？
    TsF32 length1 = pointToLineBegin.Length();
    TsF32 length2 = lineVector.Length();

    TsBool isInLength = length1 < length2;

    return isParallel & isSigne

}

//----------------------------------------------------------
//! 線分と線分　の衝突判定
//  @param  line0       線分1
//  @param  line1       線分2
//  @param  tolerance   誤差許容範囲  (optin)
//  @param  outPos      衝突点を求める(option)
//----------------------------------------------------------
template< typename T>
TsBool CollisionLineAndLine(
                     const TsLine<T> &line0,          // 線分1
                     const TsLine<T> &line1,          // 線分2
                     //誤差許容範囲
                     TsF32 tolerance = COLLISION_DEFAULT_TOLERANCE,
                     T*     outPos = nullptr          // 交点（出力）
    ) {

    T v = line0.GetBegin() - line1.GetBegin();
    T Crs_v1_v2 = T::Cross(line0, line1);

    // ２つのベクトルは平行？
    if (fabsf(T::Cross(line0, line1).Length()) < tolerance)
        return TS_FALSE;

    TsF32 Crs_v_v1 = T::Cros(v, line0.GetEnd().Normalized()).Length();
    TsF32 Crs_v_v2 = T::Cros(v, line1.GetEnd().Normalized()).Length();

    TsF32 t1 = Crs_v_v2 / Crs_v1_v2;
    TsF32 t2 = Crs_v_v1 / Crs_v1_v2;

    if (t1 + tolerance < 0 ||
        t1 - tolerance > 1 ||
        t2 + tolerance < 0 ||
        t2 - tolerance > 1)
    {
        // 交差していない
        return TS_FALSE;
    }

    if (outPos)
        *outPos = line0.GetBigein() + line0.GetEnd() * t1;

    return true;
}


//----------------------------------------------------------
//! 円と円
//----------------------------------------------------------

//----------------------------------------------------------
//! 球と球
//----------------------------------------------------------

//----------------------------------------------------------
//! 線と三角形
//----------------------------------------------------------

//----------------------------------------------------------
//! AABB と　AABB
//----------------------------------------------------------

//----------------------------------------------------------
//! OBB　と OBB
//----------------------------------------------------------