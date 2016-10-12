#pragma once

class TsFbxBone : public TsFbxNode
{
public:
    TsFbxBone(TsFbxContext* pFbxContext , TsFbxScene* pFbxScene);
    TsInt GetBoneIndex()const;
    TsBool SetBoneIndex( TsInt index );
    TsBool ComputeBindPose();
    TsMatrix GetBindPoseMatrix()
    {
        return m_bindPoseMatrixList[0];
    }
protected:
    TsInt m_boneIndex;
    TsVector<TsMatrix> m_bindPoseMatrixList;
};