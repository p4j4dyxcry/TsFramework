//**********************************************************
//! TsTransformCBuffer.h
//! オブジェクトのワールド姿勢にまつわる情報をまとめたCBuffer.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

class TsTransformCBuffer : public TsCBuffer
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsTransformCBuffer();
    virtual TsBool UpdateCBuffer( TsDeviceContext * pContext ) override;
    virtual TsBool ApplyCBuffer( TsDeviceContext * pContext) override;
    TsBool CreateTransformCBuffer( TsDevice* pDev );
    TsBool SetTransform( TsTransForm* pTransform );
protected:
    //----------------------------------------------------------
    // Define
    //----------------------------------------------------------
    struct MatrixCBuffer
    {
        TsMatrix m_MtxWorld;
        TsMatrix m_MtxInvWorld;
    };
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    TsTransForm* m_pTransform;
    TsMatrix	 m_matrixCash;
    MatrixCBuffer m_matrixCBuffer;
};