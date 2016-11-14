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
TsBool CollisionPointAndPoint(const T& p0,   //点1
                              const T& p1,   //点2
                              //誤差許容範囲
                              TsF32 tolerance = COLLISION_DEFAULT_TOLERANCE);

//----------------------------------------------------------
//! 線分と点
//  @ref 衝突点は点1と等しい
//----------------------------------------------------------
template< typename T>
TsBool CollisionRayAndPoint( const TsLine<T>& line,  //線分1
                             const T& point ,        //点1
                             // 誤差許容範囲
                             TsF32 tolerance = COLLISION_DEFAULT_TOLERANCE);

//----------------------------------------------------------
//! レイ(無限線)と点
//  @ref 衝突点は点1と等しい
//----------------------------------------------------------
template< typename T>
TsBool CollisionLineAndPoint(const TsLine<T>& ray,   //レイ
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
TsBool CollisionLineAndLine( const TsLine<T> &line0,          // 線分1
                             const TsLine<T> &line1,          // 線分2
                             //誤差許容範囲
                             TsF32 tolerance = COLLISION_DEFAULT_TOLERANCE,
                             T*     pOut     = nullptr);      // 交点（出力）
                                                                    

//----------------------------------------------------------
//! 円と点
//  @return 衝突 true 
//----------------------------------------------------------
TsBool CollisionCircleAndPoint(const TsCircle& circle,
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
TsBool CollisionSphereAndRay(const TsSphere<T>& sphere,
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
TsBool CollisionCircleAndRay(const TsCircle& circle,
                                    const TsLine2D& ray,
                                    //誤差許容範囲
                                    TsF32 tolerance = COLLISION_DEFAULT_TOLERANCE,
                                    TsVector2* pOut0 = nullptr,
                                    TsVector2* pOut1 = nullptr);

//----------------------------------------------------------
//! 円と円
//  @return 衝突 true 
//----------------------------------------------------------
TsBool CollisionCircleAndCircle(const TsCircle& c0,
                                const TsCircle& c1);
//----------------------------------------------------------
//! 球と点
//  @return 衝突 true 
//----------------------------------------------------------
TsBool CollisionSphereAndPoint( const TsSphere3D& s0,
                                const TsVector3& p0);

//----------------------------------------------------------
//! 球と球
//----------------------------------------------------------
TsBool CollisionSphereAndSphere(const TsSphere3D& s0,
                                const TsSphere3D& s1);

//----------------------------------------------------------
//! 球または円と線分の衝突判定
//  @param  s           円か球
//  @param  line        線分
//  @param  tolerance   誤差許容範囲  (optin)
//----------------------------------------------------------
template<typename T>
TsBool CollisionSphereAndLine( const TsSphere<T>& s,
                               const TsLine<T>& line,
                               //誤差許容範囲
                               TsF32 tolerance = COLLISION_DEFAULT_TOLERANCE);
//----------------------------------------------------------
//! 円と線分の衝突判定
//  @param  circle      円
//  @param  line        線分
//  @param  tolerance   誤差許容範囲  (optin)
//----------------------------------------------------------
TsBool CollisionCircleAndLine(const TsCircle& circle,
                              const TsLine2D& line,
                              //誤差許容範囲
                              TsF32 tolerance = COLLISION_DEFAULT_TOLERANCE);

//----------------------------------------------------------
//! 平面とレイの衝突判定
//  @param  normal      面のベクトル
//  @param  ray         レイ
//  @param  tolerance   誤差許容範囲  (optin)
//----------------------------------------------------------
template<typename T>
TsBool CollisionRaAndPlane(const T& normal,
                           const TsLine<T>& ray,
                           //誤差許容範囲
                           TsF32 tolerance = COLLISION_DEFAULT_TOLERANCE);
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
                                TsF32 tolerance = COLLISION_DEFAULT_TOLERANCE,
                                TsVector3* pOut = nullptr);


//----------------------------------------------------------
//! AABB と　AABB
//  @param  aabb0          AABB
//  @param  aabb1          AABB
//----------------------------------------------------------
template<typename T>
TsBool CollisionAABBAndAABB(const TsAABB<T>& aabb0,
                            const TsAABB<T>& aabb1);

//----------------------------------------------------------
//! AABB　と Ray
//----------------------------------------------------------

//----------------------------------------------------------
//! AABB　と 線分
//----------------------------------------------------------

//----------------------------------------------------------
//! OBB　と OBB
//----------------------------------------------------------

//----------------------------------------------------------
//! OBB　と Ray
//----------------------------------------------------------

//----------------------------------------------------------
//! OBB　と 線分
//----------------------------------------------------------