#include "../TsGfx.h"
TsBoneCBuffer::TsBoneCBuffer() :m_pSkeleton(nullptr)
{
}
TsBool TsBoneCBuffer::UpdateCBuffer( TsDeviceContext * pContext )
{
    if( m_pSkeleton == nullptr )
        return TS_FALSE;

    TsVector<TsBone*>& boneList = m_pSkeleton->GetBoneList();
    for each( auto pBone in boneList )
        m_boneCBuffer.bone[pBone->GetBoneID()] = pBone->GetBoneMatrix().Transposed();

    ChangedCBuffer(pContext, &m_boneCBuffer);
    return TS_TRUE;
}

TsBool TsBoneCBuffer::SetSkeleton( TsSkeleton* pSkeleton )
{
    m_pSkeleton = pSkeleton;
//    m_worldTransform = m_pRootBone;
    return TS_TRUE;
}

TsBool TsBoneCBuffer::SetWorldTransform( TsTransForm * pTransform)
{
  return TS_TRUE;
}

TsBool TsBoneCBuffer::CreateBoneCBuffer( TsDevice * pDev )
{
    CreateCBuffer(pDev, &m_boneCBuffer, SkinBoneCB, VERTEX_SHADER);

    return TS_TRUE;
}