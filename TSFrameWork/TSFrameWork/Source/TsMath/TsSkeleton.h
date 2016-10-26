#pragma once

class TsSkeleton : public TsNameObject
{
public:
    TsSkeleton();
    virtual ~TsSkeleton();
    TsBool AddBone( TsTransForm* pTransform ,
                    TsInt boneID ,
                    const TsMatrix& bindPoseMatrix ,
                    TsTransForm* pRootBone);
    TsBool IsUpdate(){ return m_isUpdate; }
    TsBool ToExecutableState(){ m_isUpdate = TS_FALSE; return TS_TRUE; };
    TsBool UpdateSkeleton();
    TsVector<TsBone*>& GetBoneList();
protected:
    TsBool m_isUpdate;
    TsVector<TsBone*> m_boneList;
};