//!*******************************************************
//! TsInstanceCBuffer.h
//!
//! InstanceCBuffer.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsInstanceCBuffer : public TsCBuffer
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsInstanceCBuffer();

    virtual TsBool UpdateCBuffer(TsDeviceContext * pContext) override;


    TsBool CreateInstanceCBuffer( TsDevice* pDev );
    TsInt SetMatrix(TsInt index, const TsMatrix& mtx);

protected:
    //----------------------------------------------------------
    // Define
    //----------------------------------------------------------
    struct InstanceCBuffer
    {
        TsMatrix instance[1024];
    };

    //----------------------------------------------------------
    // peropery
    //----------------------------------------------------------
    InstanceCBuffer m_instanceCBuffer;
};