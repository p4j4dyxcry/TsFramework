#pragma once

class TsSkeleton;

class TsBone : TsNameObject
{
    friend class TsSkeleton;
public:
    TsInt GetBoneID();
    TsTransForm* GetTransform();
    const TsMatrix& GetBoneMatrix()const;
    TsBool ComputeBoneMatrix();

    TsBool SetBindPoseMatrix( const TsMatrix& bindPoseMatrix );
    TsBool SetBoneID( TsInt id );
protected:
    TsBone*       m_RootBone;
    TsTransForm * m_pTransform;
    TsInt         m_boneID;
    TsMatrix      m_bindPoseMatrix;
    TsMatrix      m_invBindPoseMatrix;
    TsMatrix      m_boneMatrix;
};