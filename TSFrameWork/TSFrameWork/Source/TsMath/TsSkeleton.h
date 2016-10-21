#pragma once

class TsSkeleton : TsNameObject
{
public:
    TsSkeleton();
    virtual ~TsSkeleton();
    TsBool AddBone( TsTransForm* pTransform ,
                    TsInt boneID ,
                    const TsMatrix& bindPoseMatrix ,
                    TsTransForm* pRootBone);

    TsBool UpdateSkeleton();
    TsVector<TsBone*>& GetBoneList();
protected:

    TsVector<TsBone*> m_boneList;
};