#pragma once

class TsFbxBone : public TsFbxNode
{
public:
    TsFbxBone(TsFbxContext* pFbxContext , TsFbxScene* pFbxScene);
    TsInt GetBoneIndex()const;
    TsBool SetBoneIndex( TsInt index );
    TsBool ComputeBindPose();
    TsBool SetBindPose( FbxMatrix );
    TsMatrix GetBindPoseMatrix()
    {
        return m_bindPoseMatrixList[0];
    }
    TsVector<TsMatrix> m_bindPoseMatrixList;
    TsFbxBone* m_pRootBone = nullptr;

protected:
    TsInt m_boneIndex;
    
};