#include "../../TsAfx.h"

//----------------------------------------------------------
//! template�֐������I�̃C���X�^���X��
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
//! �~�Ɠ_
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
//! �~�Ɖ~
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

TsBool CollisionCircleAndRay( const TsCircle& circle,
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
//! ���Ƌ�
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
inline TsBool CollisionCircleAndLine(const TsCircle& circle,
                                     const TsLine2D& line,
                                     //�덷���e�͈�
                                     TsF32 tolerance)
{
    return CollisionSphereAndLine(circle, line, tolerance);
}
