#include "../../TsAfx.h"

TsBool TsBone::ComputeBoneMatrix()
{
    TS_HASH hash = m_RootBone->GetTransform()->GetHashCode();
    TsMatrix matrix = m_pTransform->ToLocalMatrix();
    TsTransForm* pTransform = m_pTransform->GetParent();
    while( pTransform != nullptr  )
    {
        matrix *= pTransform->ToLocalMatrix();
        if( hash == pTransform->GetHashCode() )
            break;
        pTransform = pTransform->GetParent();
    }
    m_boneMatrix = m_invBindPoseMatrix * matrix;
    return TS_TRUE;
}

TsInt TsBone::GetBoneID()
{
    return m_boneID;
}
TsTransForm* TsBone::GetTransform()
{
    return m_pTransform;
}

TsBool TsBone::SetBindPoseMatrix( const TsMatrix& bindPoseMatrix )
{
    m_bindPoseMatrix = bindPoseMatrix;
    m_invBindPoseMatrix = m_bindPoseMatrix.Inversed();
    return TS_TRUE;
}
TsBool TsBone::SetBoneID( TsInt id )
{
    m_boneID = id;
    return TS_TRUE;
}

const TsMatrix& TsBone::GetBoneMatrix()const
{
    return m_boneMatrix;
}