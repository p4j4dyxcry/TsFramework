#include "../../TsAfx.h"

//----------------------------------------------------------
//! template�֐������I�̃C���X�^���X��
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

template TsBool CollisionAABBAndPoint(const TsAABB2D&, const TsVector2&);
template TsBool CollisionAABBAndPoint(const TsAABB3D&, const TsVector3&);

template TsBool CollisionAABBAndAABB(const TsAABB2D&, const TsAABB2D&);
template TsBool CollisionAABBAndAABB(const TsAABB3D&, const TsAABB3D&);

//----------------------------------------------------------
//! �_�Ɠ_
//  @ref �Փ˓_�͓_�Ɠ�����
//----------------------------------------------------------
template< typename T>
inline TsBool CollisionPointAndPoint(const T& p0,   //�_1
                                     const T& p1,   //�_2
                                     //�덷���e�͈�
                                     TsF32 tolerance)
{
    //�Q�_�̈ʒu���قړ��������ǂ���
    return fabs((p0 - p1).Length()) < tolerance;
}

//----------------------------------------------------------
//! ���C�Ɠ_
//  @ref �Փ˓_�͓_1�Ɠ�����
//----------------------------------------------------------
template< typename T>
inline TsBool CollisionRayAndPoint( const TsLine<T>& ray,  //���C
                                    const T& point ,        //�_1
                                    // �덷���e�͈�
                                    TsF32 tolerance)
{
    T pointToLineBegin = point - ray.GetBegin();
    T lineVector = ray.GetVector();

    // �Q�̃x�N�g���͕��s�H
    return fabsf(T::Cross(pointToLineBegin, lineVector).Length()) < tolerance;
}

//----------------------------------------------------------
//! �����Ɠ_
//  @ref �Փ˓_�͓_1�Ɠ�����
//----------------------------------------------------------
template< typename T>
inline TsBool CollisionLineAndPoint(const TsLine<T>& line,  //����1
                                    const T& point ,        //�_1
                                    // �덷���e�͈�
                                    TsF32 tolerance)
{
    T pointToLineBegin = point - line.GetBegin();
    T lineVector = line.GetVector();

    // �Q�̃x�N�g���͕��s�H
    TsBool isParallel = fabsf(T::Cross(pointToLineBegin, lineVector).Length()) < tolerance;

    //�x�N�g���̌����͔��΂������Ă��Ȃ��H
    TsBool isSigne = T::Dot(pointToLineBegin, lineVector) >= 0;

    //�_������̎��_�܂ł̒������I�_�𒴂��Ă��Ȃ��H
    TsF32 length1 = pointToLineBegin.Length();
    TsF32 length2 = lineVector.Length();

    TsBool isInLength = length1 < length2;

    return isParallel & isSigne;
}

//----------------------------------------------------------
//! �����Ɛ����@�̏Փ˔���
//  @param  line0       ����1
//  @param  line1       ����2
//  @param  tolerance   �덷���e�͈�  (optin)
//  @param  pOut        �Փ˓_�����߂�(option)
//----------------------------------------------------------
template< typename T>
inline TsBool CollisionLineAndLine( const TsLine<T> &line0, // ����1
                                    const TsLine<T> &line1, // ����2                                    
                                    TsF32 tolerance,        //�덷���e�͈�
                                    T*     pOut)            // ��_(�o��)
{

    T v = line0.GetBegin() - line1.GetBegin();
    T Crs_v1_v2 = T::Cross(line0.GetVector(), line1.GetVector());

    // �Q�̃x�N�g���͕��s�H
    if (fabsf(Crs_v1_v2.Length()) < tolerance)
        return TS_FALSE;

    // �x�N�g���̊O�ς���sin���擾
    TsF32 t1 = TsComputeSin(v, line0.GetEnd());
    TsF32 t2 = TsComputeSin(v, line1.GetEnd());

    if (t1 + tolerance < 0 ||
        t1 - tolerance > 1 ||
        t2 + tolerance < 0 ||
        t2 - tolerance > 1)
    {
        // �������Ă��Ȃ�
        return TS_FALSE;
    }

    if (pOut != nullptr)
        *pOut = line0.GetBegin() + line0.GetEnd() * t1;

    return TS_TRUE;
}

//----------------------------------------------------------
//! �~or���Ɠ_
//  @return �Փ� true 
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
//! �~�Ɠ_
//----------------------------------------------------------
inline TsBool CollisionSphere2DAndPoint(const TsCircle& circle,
                                        const TsVector2& point)
{
    return CollisionSphereAndPoint(circle,point);
}

//----------------------------------------------------------
//! �~�Ɖ~
//----------------------------------------------------------
inline TsBool CollisionSphereAndSphere2D(const TsCircle& c0,
                                       const TsCircle& c1)
{
    return CollisionSphereAndSphere(c0,c1);
}

//----------------------------------------------------------
//! ���܂��͉~�Ɛ����̏Փ˔���
//  @param  sphere      ���܂��͉~
//  @param  ray         ���C(�����ɑ�����)
//  @param  tolerance   �덷���e�͈�  (optin)
//  @param  pOut1       �ߌi�̏Փ˓_�����߂�(option)
//  @param  pOut2       ���i�̏Փ˓_�����߂�(option)
//----------------------------------------------------------
template< typename T>
inline TsBool CollisionSphereAndRay(const TsSphere<T>& sphere,
                                     const TsLine<T>& line,
                                     //�덷���e�͈�
                                     TsF32 tolerance ,
                                     T* pOut0,
                                     T* pOut1)
{
    T&& offset = line.GetBegin() - sphere.GetCenter();
    T&& vec    = line.GetNormalizeVector();
    TsF32 radius = sphere.GetRadius();

    // �W��t���Z�o
    TsF32 cos01 = T::Dot(offset, vec);
    TsF32 cos02 = T::Dot(offset, offset);

    TsF32 s = cos01 * cos01 - cos02 + radius * radius;

    // �덷���C��
    if (TsAbs(s) < tolerance)
        s = 0.0f;
    else if (s < 0.0f)
        return TS_FALSE;

    TsF32 sq = sqrtf(s);
    TsF32 t0 = -cos01 - sq;
    TsF32 t1 = -cos01 + sq;

    if (t0 < 0 || t1 < 0)
        return TS_FALSE;

    // �Փˍ��W���o��
    if (pOut0)
        *pOut0 = offset + sphere.GetCenter() + vec * t0;
    if (pOut1)
        *pOut1 = offset + sphere.GetCenter() + vec * t1;
    
    return TS_TRUE;
}

//----------------------------------------------------------
//! ���Ɛ����̏Փ˔���
//  @param  circle      �~
//  @param  ray         ���C(�����ɑ�����)
//  @param  tolerance   �덷���e�͈�  (optin)
//  @param  pOut1       �ߌi�̏Փ˓_�����߂�(option)
//  @param  pOut2       ���i�̏Փ˓_�����߂�(option)
//----------------------------------------------------------

TsBool CollisionSphereAndRay2D( const TsCircle& circle,
                              const TsLine2D& line,
                              //�덷���e�͈�
                              TsF32 tolerance ,
                              TsVector2* pOut0,
                              TsVector2* pOut1)
{
    return CollisionSphereAndRay(circle,line,tolerance,pOut0,pOut1);
}

//----------------------------------------------------------
//! ���Ɠ_
//  @return �Փ� true 
//----------------------------------------------------------
inline TsBool CollisionSphereAndPoint3D( const TsSphere3D& s0,
                                         const TsVector3& p0)
{
    return CollisionSphereAndPoint(s0, p0);
}

//----------------------------------------------------------
//! �~�Ɖ~ or ���Ƌ�
//  @return �Փ� true 
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
//! ���Ƌ�
//----------------------------------------------------------
inline TsBool CollisionSphereAndSphere3D(const TsSphere3D& s0,
                                       const TsSphere3D& s1)
{
    return CollisionSphereAndSphere(s0, s1);
}


//----------------------------------------------------------
//! ���܂��͉~�Ɛ����̏Փ˔���
//  @param  s           �~����
//  @param  line        ����
//  @param  tolerance   �덷���e�͈�  (optin)
//----------------------------------------------------------
template<typename T>
inline TsBool CollisionSphereAndLine( const TsSphere<T>& s,
                                      const TsLine<T>& line,
                                     //�덷���e�͈�
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
//! �~�Ɛ����̏Փ˔���
//  @param  circle      �~
//  @param  line        ����
//  @param  tolerance   �덷���e�͈�  (optin)
//----------------------------------------------------------
inline TsBool CollisionSphereAndLine2D(const TsCircle& circle,
                                       const TsLine2D& line,
                                     //�덷���e�͈�
                                     TsF32 tolerance)
{
    return CollisionSphereAndLine(circle, line, tolerance);
}


//----------------------------------------------------------
//! ���ʂƃ��C�̏Փ˔���
//  @param  normal      �ʂ̃x�N�g��
//  @param  ray         ���C
//  @param  tolerance   �덷���e�͈�  (optin)
//----------------------------------------------------------
template<typename T>
inline TsBool CollisionRayAndPlane(const T& normal,
                                  const TsLine<T>& ray,
                                  //�덷���e�͈�
                                  TsF32 tolerance )
{
    return T::Dot(normal, ray.GetVector()) != 0;
}


//----------------------------------------------------------
//! �����ƎO�p�`
//  @param  p0          ���_
//  @param  p1          ���_
//  @param  p2          ���_
//  @param  line        ����
//  @param  tolerance   �덷���e�͈�  (optin)
//  @param  pOut        �����_
//----------------------------------------------------------
TsBool CollisionLineAndTriangle(const TsVector3& p0,
                                const TsVector3& p1,
                                const TsVector3& p2,
                                const TsLine3D& line,
                                //�덷���e�͈�
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

    // 1�ڂ̕ӂ̒��ɔ[�܂��Ă��邩����
    {
        d0 = p - p0;
        d1 = p1 - p0;

        cross = TsVector3::Cross(d0, d1);
        if (TsVector3::Dot(cross, normal) < 0)
            return TS_FALSE;
    }

    // 2�ڂ̕ӂ̒��ɔ[�܂��Ă��邩����
    {
        d0 = p - p1;
        d1 = p2 - p1;

        cross = TsVector3::Cross(d0, d1);
        if (TsVector3::Dot(cross, normal) < 0)
            return TS_FALSE;
    }

    // 3�ڂ̕ӂ̒��ɔ[�܂��Ă��邩����
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
//! AABB �Ɓ@AABB
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

    // ���������v�Z
    TsInt sz = sizeof(T) / sizeof(TsF32);

    for (TsInt i = 0; i < sz; ++i)
    {
        //! �������Փ˂��Ă��邩����
        //�@������ x -> y -> z -> w ...etc�@�Ɓ������܂Ŕ���ł���B
        if ( aMin[i] > bMax[i] || bMin[i] > aMax[i] )
            return TS_FALSE;
    }
    return TS_TRUE;
}

//=====================================================
// * �y���z��̊֐��Ōv�Z�ł���悤�ɂ���
//=====================================================
//template<> //2d
//TsBool CollisionAABBAndAABB( const TsAABB2D& a,
//                             const TsAABB2D& b)
//{
//    //! ���� 2d��3d����̊֐��ōs����悤�ɂ���H
//    //�@�����
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
//! AABB �Ɓ@Ray
//  @param  aabb           AABB
//  @param  rayDir         ���C
//  @prama  tolerance      �덷���e�͈�
//  @param  pOut           �Փ˓_(option)
//----------------------------------------------------------
template<typename T>
TsBool CollisionAABBAndRay (const TsAABB<T>& aabb,
                            const TsLine<T>& ray,
                            TsF32 tolerance,
                            T * pOut )
{
    // �x�N�g���̎����ɍ��킹���v�f���̎擾
    // Vector2 -> 2
    // Vector3 -> 3
    TsInt sz = sizeof(T) / sizeof(TsF32);

    const T& rayPos = ray.GetBegin();
    const T& rayDir = ray.GetNormalizeVector();
    const T& min = aabb.GetMin();
    const T& max = aabb.GetMax();

    TsF32 t = -FLT_MAX;
    TsF32 t_max = FLT_MAX;

    //! ��������
    for (int i = 0; i < sz; ++i)
    {
        if ( fabsf(rayDir[i]) < tolerance)
        {
            if (rayPos[i] < min[i] || rayPos[i] >max[i])
                return TS_FALSE; // �������Ă��Ȃ�
        }
        else 
        {
            // �X���u�Ƃ̋������Z�o
            // t1���߃X���u�At2�����X���u�Ƃ̋���
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

            // �X���u�����`�F�b�N
            if (t >= t_max)
                return TS_FALSE;
        }
    }

    // �Փ˓_�����߂�
    if (pOut) 
    {
        *pOut = rayPos + t * rayDir;
    }

    return TS_TRUE;
}

//----------------------------------------------------------
//! AABB�@�� ����
//  @param  aabb           AABB
//  @param  line           ����
//  @prama  tolerance      �덷���e�͈�
//  @param  pOut           �Փ˓_(option)
//----------------------------------------------------------
template<typename T>
TsBool CollisionAABBAndLine(const TsAABB<T>& aabb,
                            const TsSphere<T>& Line,
                            TsF32 tolerance ,
                            T * pOut)
{
    //�Փ˓_t
    T t;
    // ���C�Ɛ����̂����蔻������߂�
    TsBool bRayAndAABB = 
        CollisionAABBAndRay(aabb, 
                            Line, 
                            tolerance, 
                            &t);

    // ���C���Փ˂��Ă��Ȃ�������Փ˂��Ȃ�
    if (bRayAndAABB == TS_FALSE)
        return TS_FALSE;

    // �Փ˓_�Ɛ����̂����蔻������߂�
    TsBool isLineAndPoint =
        CollisionLineAndPoint(Line, t, tolerance);

    if ( isLineAndPoint && pOut )
        *pOut = t;

    return isLineAndPoint;
}
//----------------------------------------------------------
//! AABB�@�� �_
//----------------------------------------------------------
template<typename T>
TsBool CollisionAABBAndPoint(const TsAABB<T>& aabb,
                             const T& point)
{
    const T& aMin = aabb.GetMin();
    const T& aMax = aabb.GetMax();

    // ���������v�Z
    TsInt sz = sizeof(T) / sizeof(TsF32);

    for (TsInt i = 0; i < sz; ++i)
    {
        //! �������Փ˂��Ă��邩����
        //�@������ x -> y -> z -> w ...etc�@�Ɓ������܂Ŕ���ł���B
        if (aMin[i] > point[i] || point[i] > aMax[i])
            return TS_FALSE;
    }
    return TS_TRUE;
}

//----------------------------------------------------------
//! AABB�@�� �~or��
//----------------------------------------------------------
template<typename T>
TsBool CollisionAABBAndSphere(const TsAABB<T>& aabb,
                              const TsSphere<T>& sphere)
{
    TsVector<T>&& v = aabb.GetVertexList();

    for (auto it : v)
    {
        //�S�Ă�AABB�̒��_�Ɖ~or���̂����蔻����s��
        if ( CollisionSphereAndPoint(sphere, it) )
            return TS_TRUE;
    }
    return TS_FALSE;
}

//----------------------------------------------------------
//! OBB�@�� �_
//----------------------------------------------------------
TsBool CollisionOBBAndPoint(const TsOBB& obb,
                            const TsVector3& pt,
                            TsF32 tolerance )
{
    TsVector3 p = obb.GetCenter();
    TsVector3 v = pt - p;

    TsMatrix m = obb.GetRotate().ToMatrix();

    TsVector3 axis[3] =
    {
        TsVector3(m._11, m._12, m._13), //xAxis
        TsVector3(m._21, m._22, m._23), //yAxis
        TsVector3(m._31, m._32, m._33)  //zAxis
    };
    TsVector3 scale = obb.GetScale();

    //OBB �Ɠ_�̍Őڋߓ_�����߂�
    TsF32 f;
    for (TsInt i = 0; i < 3; i++)
    {
        f = TsVector3::Dot(v, axis[i]);

        if (f > scale[i])
        {
            f = scale[i];
        }
        if (f < -scale[i])
        {
            f = -scale[i];
        }
        p += f * axis[i];
    }
    TsVector3 d = p - pt;

    //�덷���e�͈͓��Ȃ�Փ˂��Ă���Ƃ���
    return TsVector3::Dot(d, d) <= tolerance;
}

//----------------------------------------------------------
//! OBB�@�� ��
//----------------------------------------------------------
TsBool CollisionOBBAndSphere(const TsOBB& obb,
                             const TsSphere3D& sphere)
{
    TsVector3 p = obb.GetCenter();
    TsVector3 v = sphere.GetCenter() - p;

    TsMatrix m = obb.GetRotate().ToMatrix();

    TsVector3 axis[3] =
    {
        TsVector3(m._11, m._12, m._13), //xAxis
        TsVector3(m._21, m._22, m._23), //yAxis
        TsVector3(m._31, m._32, m._33)  //zAxis
    };
    TsVector3 scale = obb.GetScale();

    //OBB �Ɠ_�̍Őڋߓ_�����߂�
    TsF32 f;
    for (TsInt i = 0; i < 3; i++)
    {
        f = TsVector3::Dot(v, axis[i]);

        if (f > scale[i])
        {
            f = scale[i];
        }
        if (f < -scale[i])
        {
            f = -scale[i];
        }
        p += f * axis[i];
    }
    TsVector3 d = p - sphere.GetCenter();

    TsF32 r = sphere.GetRadius();

    return TsVector3::Dot(d, d) <= r*r;
}

//----------------------------------------------------------
//! OBB�@�� Ray
//----------------------------------------------------------
TsBool CollisionOBBAndRay(const TsOBB& obb,
                          const TsLine3D& ray,
                          TsF32 tolerance )
{
    TsMatrix m = obb.GetRotate().ToMatrix();

    TsVector3 axis[3] =
    {
        TsVector3(m._11, m._12, m._13), //xAxis
        TsVector3(m._21, m._22, m._23), //yAxis
        TsVector3(m._31, m._32, m._33)  //zAxis
    };
    TsVector3 scale = obb.GetScale();

    //todo

    return TS_TRUE
}

//----------------------------------------------------------
//! OBB�@�� ����
//----------------------------------------------------------
TsBool CollisionOBBAndRay   ( const TsOBB& obb,
                              const TsLine3D& line ,
                              TsF32 tolerance )
{
    //TsVector3 m = (ray->org + ray->dir) * 0.5f;
    //TsVector3 d = ray->dir - m;
    //m = m - obb->c;
    //m = D3DXVECTOR3(D3DXVec3Dot(&obb->u[0], &m), D3DXVec3Dot(&obb->u[1], &m), D3DXVec3Dot(&obb->u[2], &m));
    //d = D3DXVECTOR3(D3DXVec3Dot(&obb->u[0], &d), D3DXVec3Dot(&obb->u[1], &d), D3DXVec3Dot(&obb->u[2], &d));

    //float adx = fabsf(d.x);
    //if (fabsf(m.x) > obb->e.x + adx) return 0;
    //float ady = fabsf(d.y);
    //if (fabsf(m.y) > obb->e.y + ady) return 0;
    //float adz = fabsf(d.z);
    //if (fabsf(m.z) > obb->e.z + adz) return 0;
    //adx += EPSILON;
    //ady += EPSILON;
    //adz += EPSILON;

    //if (fabsf(m.y * d.z - m.z * d.y) > obb->e.y * adz + obb->e.z * ady) return 0;
    //if (fabsf(m.z * d.x - m.x * d.z) > obb->e.x * adz + obb->e.z * adx) return 0;
    //if (fabsf(m.x * d.y - m.y * d.x) > obb->e.x * ady + obb->e.y * adx) return 0;

    //return 1;

    return TS_TRUE;
}

//----------------------------------------------------------
//! OBB�@�� OBB
//----------------------------------------------------------
TsBool CollisionOBBAndOBB   ( const TsOBB& obb,
                              const TsOBB& line)
{
    TsMatrix m = obb.GetRotate().ToMatrix();

    TsVector3 axis[3] =
    {
        TsVector3(m._11, m._12, m._13), //xAxis
        TsVector3(m._21, m._22, m._23), //yAxis
        TsVector3(m._31, m._32, m._33)  //zAxis
    };
    TsVector3 scale = obb.GetScale();

    //todo

    return TS_TRUE;
}