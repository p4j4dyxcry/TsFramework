//!*******************************************************
//! TsFbxBone.h
//!
//! Fbx Skeleton lapper.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsFbxBone : public TsFbxNode
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsFbxBone(TsFbxContext* pFbxContext , TsFbxScene* pFbxScene);
    TsInt GetBoneIndex()const;
    TsBool SetBoneIndex( TsInt index );
    TsBool ComputeBindPose();
    TsBool SetBindPose( FbxMatrix );
    TsMatrix GetBindPoseMatrix();

protected:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    TsInt m_boneIndex;
    TsVector<TsMatrix> m_bindPoseMatrixList;
    TsFbxBone* m_pRootBone = nullptr;

};
