//**********************************************************
//! TsBoneCBuffer.h
//! Bone CBuffer.
//!
//! © 2016 Yuki Tsuneyama

#pragma once
//----------------------------------------------------------
// Define
//----------------------------------------------------------
struct BoneCBuffer
{
	TsMatrix bone[512];
};
class TsBoneCBuffer : public TsCBuffer<BoneCBuffer>
{
public:
    //! Constructer
    TsBoneCBuffer();

    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    virtual TsBool UpdateCBuffer( TsDeviceContext * pContext ) override;
    TsBool SetSkeleton( TsSkeleton* pTransform );
    TsBool SetWorldTransform( TsTransForm* pTransform );

    TsBool CreateBoneCBuffer( TsDevice* pDev );

protected:


    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    TsSkeleton*         m_pSkeleton;
    TsTransForm*        m_worldTransform;
};