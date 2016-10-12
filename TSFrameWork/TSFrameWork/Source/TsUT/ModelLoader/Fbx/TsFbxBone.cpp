#include "../../TsUT.h"
#include "TsFbxHeader.h"

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

TsBool TsFbxBone::ComputeBindPose()
{
    TsMatrix* pBindPoseMatrix = m_pFbxScene->GetFirstBindPoseMatrix(GetName());
    m_bindPoseMatrixList.resize(1);
   
    TsTransForm transform;
    FbxMatrix baseposeMatrix = m_fbxNode->EvaluateGlobalTransform();
    transform = FbxMatrixToTsMatrix( baseposeMatrix );

    transform.m_localPosition.x *= -1;
    transform.m_localRotate.x *= -1;
    transform.m_localRotate.w *= -1;
    m_bindPoseMatrixList[0] = transform.ToLocalMatrix();
    //((TsBoneTransForm*)(m_pTransform))->SetBasePoseInv(m_bindPoseMatrixList[0]);
    //((TsBoneTransForm*)(m_pTransform))->SetID(m_boneIndex);

    return TS_TRUE;
}