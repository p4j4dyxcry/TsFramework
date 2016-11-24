#include "../../TsAfx.h"

TsOBB::TsOBB(const TsVector3&      center,
             const TsVector3&      scale,
             const TsQuaternion&   quaternion)
             :m_center(center)
             ,m_scale(scale)
             , m_quaternion(quaternion)
             
{}

TsOBB::~TsOBB(){};

const TsQuaternion& TsOBB::GetRotate()const
{
    return m_quaternion;
}
TsQuaternion& TsOBB::GetRotate()
{
    return m_quaternion;
}
const TsVector3& TsOBB::GetCenter()const
{
    return m_center;
}
TsVector3& TsOBB::GetCenter()
{
    return m_center;
}
const TsVector3& TsOBB::GetScale()const
{
    return m_scale;
}

TsVector3& TsOBB::GetScale()
{
    return m_scale;
}

TsBool TsOBB::SetCenter(const TsVector3& v)
{
    m_center = v;
    return TS_TRUE;
}
TsBool TsOBB::SetScale(const TsVector3& v)
{
    m_scale = v;
    return TS_TRUE;

}
TsBool TsOBB::SetRotate(const TsQuaternion& q)
{
    m_quaternion = q;
    return TS_TRUE;
}