#include "TsFbxAfx.h"

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

TsBool TsFbxBone::SetBindPose( TsMatrix& baseposeMatrix )
{
    m_bindPoseMatrix = baseposeMatrix;
    return TS_TRUE;
}

TsMatrix TsFbxBone::GetBindPoseMatrix()
{
    return m_bindPoseMatrix;
}
