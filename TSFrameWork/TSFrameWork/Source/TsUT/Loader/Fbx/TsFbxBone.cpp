#include "TsFbxAfx.h"

TsFbxBone::TsFbxBone(TsFbxContext * pFbxContext, TsFbxScene * pFbxScene)
:TsFbxNode( pFbxContext , pFbxScene )
{
    m_boneIndex = -1;
    m_bindPoseMatrixList.reserve(32);
}

TsBool TsFbxBone::SetBoneIndex(TsInt index)
{
    m_boneIndex = index;
    return TS_TRUE;
}


TsInt TsFbxBone::GetBoneIndex()const
{
    return m_boneIndex;
}

TsBool TsFbxBone::SetBindPose( TsMatrix& baseposeMatrix )
{
    m_bindPoseMatrixList.resize( 1 );
    m_bindPoseMatrixList[0] = baseposeMatrix;

    return TS_TRUE;
}

TsMatrix TsFbxBone::GetBindPoseMatrix()
{
    if (m_bindPoseMatrixList.empty() )
    {
        m_bindPoseMatrixList.resize(1);
//        m_bindPoseMatrixList[0] = m_pTransform->ToWorldMatrix();
        m_bindPoseMatrixList[0] = TsMatrix::identity;
    }

    return m_bindPoseMatrixList[0];
}

TsBool TsFbxBone::ComputeBindPose()
{
#if 0
    TsMatrix* pBindPoseMatrix = m_pFbxScene->GetBindPoseMatrix(GetName());
    m_bindPoseMatrixList.resize(1);

    if( pBindPoseMatrix )
    {
        TsTransForm t = *pBindPoseMatrix;
        t.m_localPosition.x *= -1;
        t.m_localRotate.x *= -1;
        t.m_localRotate.w *= -1;
        m_bindPoseMatrixList[0] = t.ToLocalMatrix();
        return TS_TRUE;
    }
    
    TsFbxNode * p = this;
    TsMatrix m = TsMatrix::identity;

    while( p )
    {
        if( p->IsSkeleton() )
            m *= p->GetTransform()->ToLocalMatrix();
        else
            break;
        if( m_parent  )
            p = p->GetParent();
        else
            break;
    }

    m_bindPoseMatrixList.resize( 1 );
    m_bindPoseMatrixList[0] = m;
#else
//    m_bindPoseMatrixList.resize( 1 );
//    m_bindPoseMatrixList[0] = m_pTransform->ToWorldMatrix();
#endif

    //((TsBoneTransForm*)(m_pTransform))->SetBasePoseInv(m_bindPoseMatrixList[0]);
    //((TsBoneTransForm*)(m_pTransform))->SetID(m_boneIndex);

    return TS_TRUE;
}