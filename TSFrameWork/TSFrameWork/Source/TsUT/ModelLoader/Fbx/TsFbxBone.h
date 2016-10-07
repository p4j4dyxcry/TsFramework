#pragma once

class TsFbxBone : public TsFbxNode
{
public:
    TsFbxBone(TsFbxContext* pFbxContext , TsFbxScene* pFbxScene);
    TsInt GetBoneIndex()const;
    TsBool SetBoneIndex( TsInt index );
    TsBool ComputeBindPose();
protected:
    TsInt m_boneIndex;
    TsMatrix m_bindPoseMatrix;
};