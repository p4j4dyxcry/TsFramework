#include "../../TsUT.h"
#include "TsFbxHeader.h"

TsFbxBone::TsFbxBone(TsFbxContext * pFbxContext, TsFbxScene * pFbxScene)
:TsFbxNode( pFbxContext , pFbxScene )
{
    m_boneIndex = -1;
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

TsBool TsFbxBone::ComputeBindPose()
{
    //todo 本当はボーンのルートから計算する必要がある。
    FbxMatrix baseposeMatrix = m_fbxNode->EvaluateGlobalTransform();
    TsTransForm transform = FbxMatrixToTsMatrix( baseposeMatrix ).Inverse();
    transform.m_localPosition.x *= -1;
    transform.m_localRotate.x *= -1;
    transform.m_localRotate.w *= -1;
    m_bindPoseMatrix = transform.ToLocalMatrix();
    ( ( TsBoneTransForm* )( m_pTransform ) )->SetBasePoseInv( m_bindPoseMatrix );
    ( ( TsBoneTransForm* )( m_pTransform ) )->ToBoneMatrix( TsMatrix::identity );
    ( ( TsBoneTransForm* )( m_pTransform ) )->SetID(m_boneIndex);
    return TS_TRUE;
}