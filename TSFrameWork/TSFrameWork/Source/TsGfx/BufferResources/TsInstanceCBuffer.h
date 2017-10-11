//!*******************************************************
//! TsInstanceCBuffer.h
//!
//! InstanceCBuffer.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

//----------------------------------------------------------
// Define
//----------------------------------------------------------
struct InstanceCBuffer
{
	TsMatrix instance[1024];
};
class TsInstanceCBuffer : public TsCBuffer<InstanceCBuffer>
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
};