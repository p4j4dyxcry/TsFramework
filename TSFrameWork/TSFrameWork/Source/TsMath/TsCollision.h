//!*******************************************************
//! TsCollision.h
//!
//! コリジョンのユーティリティ.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

//コリジョンの許容する誤差の規定値
static const TsF32 COLLISION_DEFAULT_TOLERANCE = 0.000001f;

//----------------------------------------------------------
//! 点と点
//  @ref 衝突点は点と等しい
//  @return 衝突 true 
//----------------------------------------------------------
template< typename T>
inline TsBool CollisionPointAndPoint(const T& p0,   //点1
                                     const T& p1,   //点2
                                     //誤差許容範囲
                                     TsF32 tolerance = COLLISION_DEFAULT_TOLERANCE);

//----------------------------------------------------------
//! 線分と点
//  @ref 衝突点は点1と等しい
//----------------------------------------------------------
template< typename T>
inline TsBool CollisionLineAndPoint(const TsLine<T>& line,  //線分1
                                    const T& point ,        //点1
                                    // 誤差許容範囲
                                    TsF32 tolerance = COLLISION_DEFAULT_TOLERANCE);

//----------------------------------------------------------
//! 線分と線分　の衝突判定
//  @param  line0       線分1
//  @param  line1       線分2
//  @param  tolerance   誤差許容範囲  (optin)
//  @param  pOut        衝突点を求める(option)
//  @return 衝突 true 
//----------------------------------------------------------
template< typename T>
inline TsBool CollisionLineAndLine( const TsLine<T> &line0,          // 線分1
                                    const TsLine<T> &line1,          // 線分2
                                    //誤差許容範囲
                                    TsF32 tolerance = COLLISION_DEFAULT_TOLERANCE,
                                    T*     pOut     = nullptr);      // 交点（出力）
                                                                    

//----------------------------------------------------------
//! 円と点
//  @return 衝突 true 
//----------------------------------------------------------
inline TsBool CollisionCircleAndPoint(const TsCircle& circle,
                                      const TsVector2& point);


//----------------------------------------------------------
//! 球と線分の衝突判定
//  @param  sphere      球
//  @param  ray         レイ(無限に続く線)
//  @param  tolerance   誤差許容範囲  (optin)
//  @param  pOut1       近景の衝突点を求める(option)
//  @param  pOut2       遠景の衝突点を求める(option)
//  @return 衝突 true 
//----------------------------------------------------------
template< typename T>
inline TsBool CollisionSphereAndRay(const TsSphere<T>& sphere,
                                    const TsLine<T>& ray,
                                    //誤差許容範囲
                                    TsF32 tolerance = COLLISION_DEFAULT_TOLERANCE,
                                    T* pOut0 = nullptr,
                                    T* pOut1 = nullptr);

//----------------------------------------------------------
//! 球と線分の衝突判定
//  @param  circle      円
//  @param  ray         レイ(無限に続く線)
//  @param  tolerance   誤差許容範囲  (optin)
//  @param  pOut1       近景の衝突点を求める(option)
//  @param  pOut2       遠景の衝突点を求める(option)
//  @return 衝突 true 
//----------------------------------------------------------
inline TsBool CollisionCircleAndRay(const TsCircle& circle,
                                    const TsLine2D& ray,
                                    //誤差許容範囲
                                    TsF32 tolerance = COLLISION_DEFAULT_TOLERANCE,
                                    TsVector2* pOut0 = nullptr,
                                    TsVector2* pOut1 = nullptr);

//----------------------------------------------------------
//! 円と円
//  @return 衝突 true 
//----------------------------------------------------------
inline TsBool CollisionCircleAndCircle(const TsCircle& c0,
                                       const TsCircle& c1);
//----------------------------------------------------------
//! 球と点
//  @return 衝突 true 
//----------------------------------------------------------
inline TsBool CollisionSphereAndPoint( const TsSphere3D& s0,
                                       const TsVector3& p0);

//----------------------------------------------------------
//! 球と球
//----------------------------------------------------------
inline TsBool CollisionSphereAndSphere(const TsSphere3D& s0,
                                       const TsSphere3D& s1);

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
                                     TsF32 tolerance = COLLISION_DEFAULT_TOLERANCE);
//----------------------------------------------------------
//! 円と線分の衝突判定
//  @param  circle      円
//  @param  line        線分
//  @param  tolerance   誤差許容範囲  (optin)
//----------------------------------------------------------
inline TsBool CollisionCircleAndLine(const TsCircle& circle,
                                     const TsLine2D& line,
                                     //誤差許容範囲
                                     TsF32 tolerance = COLLISION_DEFAULT_TOLERANCE);
//----------------------------------------------------------
//! 線と三角形
//----------------------------------------------------------

//----------------------------------------------------------
//! AABB と　AABB
//----------------------------------------------------------

//----------------------------------------------------------
//! OBB　と OBB
//----------------------------------------------------------