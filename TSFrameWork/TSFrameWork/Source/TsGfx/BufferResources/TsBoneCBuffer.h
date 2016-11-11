//**********************************************************
//! TsBoneCBuffer.h
//! Bone CBuffer.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

class TsBoneCBuffer : public TsCBuffer
{
public:
    //! Constructer
    TsBoneCBuffer();

    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    virtual TsBool UpdateCBuffer( TsDeviceContext * pContext ) override;
    virtual TsBool ApplyCBuffer( TsDeviceContext * pContext ) override;
    TsBool SetSkeleton( TsSkeleton* pTransform );
    TsBool SetWorldTransform( TsTransForm* pTransform );

    TsBool CreateBoneCBuffer( TsDevice* pDev );

protected:
    //----------------------------------------------------------
    // Define
    //----------------------------------------------------------
    struct BoneCBuffer
    {
        TsMatrix bone[512];
    };

    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    TsSkeleton*         m_pSkeleton;
    TsTransForm*        m_worldTransform;
    BoneCBuffer         m_boneCBuffer;
    
};