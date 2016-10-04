#pragma once

class TsFbxBone : public TsFbxNode
{
public:
	TsFbxBone(TsFbxContext* pFbxContext , TsFbxScene* pFbxScene);
	TsInt GetBoneIndex()const;
	TsBool SetBoneIndex( TsInt index );
protected:
	TsInt m_boneIndex;
};