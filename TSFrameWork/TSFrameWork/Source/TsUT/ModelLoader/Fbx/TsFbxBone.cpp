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
    FbxMatrix baseposeMatrix = m_fbxNode->EvaluateGlobalTransform().Inverse();
    m_bindPoseMatrix = FbxMatrixToTsMatrix( baseposeMatrix );

    return TS_TRUE;
}