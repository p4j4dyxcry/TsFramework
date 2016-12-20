//**********************************************************
//! TsSkeleton.h
//! bone の集合.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

class TsSkeleton : public TsNameObject
{
public:
    //----------------------------------------------------------
    //! Constructor
    //----------------------------------------------------------
    TsSkeleton();

    //----------------------------------------------------------
    //! Destructor
    //----------------------------------------------------------
    virtual ~TsSkeleton();

    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsBool AddBone( TsTransForm* pTransform ,
                    TsInt boneID ,
                    const TsMatrix& bindPoseMatrix );
    TsBool IsUpdate(){ return m_isUpdate; }
    TsBool ToExecutableState(){ m_isUpdate = TS_FALSE; return TS_TRUE; };
    TsBool UpdateSkeleton();
    TsVector<TsBone*>& GetBoneList();
protected:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    TsBool m_isUpdate;
    TsVector<TsBone*> m_boneList;
};