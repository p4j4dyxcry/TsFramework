#include "../../TsAfx.h"

TsBool TsBone::ComputeBoneMatrix()
{
#if 0
    TS_HASH hash = m_RootBone->GetHashCode();
    TsMatrix boneMatrix = TsMatrix::identity;
    TsMatrix treeMatrix = TsMatrix::identity;
    TsTransForm* pTransform = m_pTransform;
    while( pTransform != nullptr  )
    {
        boneMatrix *= pTransform->ToLocalMatrix();
        if( hash == pTransform->GetHashCode() )
            break;
        pTransform = pTransform->GetParent();
    }
    if( pTransform )
        pTransform = pTransform->GetParent();
    while( pTransform != nullptr )
    {
        treeMatrix *= pTransform->ToLocalMatrix();
        pTransform = pTransform->GetParent();
    }
    m_boneMatrix = treeMatrix *( m_invBindPoseMatrix * boneMatrix );
#else
    m_boneMatrix = ( m_invBindPoseMatrix * m_pTransform->ToWorldMatrix() );
#endif
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