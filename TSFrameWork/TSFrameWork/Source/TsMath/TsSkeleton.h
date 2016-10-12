#pragma once

class TsSkeleton : TsNameObject
{
public:
    TsSkeleton();
    virtual ~TsSkeleton();
    TsBool AddBone( TsTransForm* pTransform ,
                    TsInt boneID ,
                    const TsMatrix& bindPoseMatrix );
    TsBool AddRootBone( TsTransForm* pTransform ,
                        TsInt boneID ,
                        const TsMatrix& bindPoseMatrix);
    TsBool SetRootBoneByID(TsInt id);

    TsBool UpdateSkeleton();
    TsVector<TsBone*>& GetBoneList();
protected:
    TsBool UpdateBone( TsBone *);
    TsVector<TsBone*> m_boneList;
    TsBone*           m_rootBone;
};