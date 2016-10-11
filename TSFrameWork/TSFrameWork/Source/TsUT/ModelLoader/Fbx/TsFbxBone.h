#pragma once

class TsFbxBone : public TsFbxNode
{
public:
    TsFbxBone(TsFbxContext* pFbxContext , TsFbxScene* pFbxScene);
    TsInt GetBoneIndex()const;
    TsBool SetBoneIndex( TsInt index );
    TsBool ComputeBindPose();
    TsBool AddCluster(FbxCluster* pFbxCluster);
protected:
    TsInt m_boneIndex;
    TsVector<TsMatrix> m_bindPoseMatrixList;
    TsVector<FbxCluster*> m_pClusterList;
};