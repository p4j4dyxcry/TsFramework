﻿//**********************************************************
//! TsTransformCBuffer.h
//! オブジェクトのワールド姿勢にまつわる情報をまとめたCBuffer.
//!
//! © 2016 Yuki Tsuneyama

#pragma once
//----------------------------------------------------------
// Define
//----------------------------------------------------------
struct MatrixCBuffer
{
	TsMatrix m_MtxWorld;
};
class TsTransformCBuffer : public TsCBuffer<MatrixCBuffer>
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsTransformCBuffer();
    virtual TsBool UpdateCBuffer( TsDeviceContext * pContext ) override;
    TsBool CreateTransformCBuffer( TsDevice* pDev );
    TsBool SetTransform( TsTransForm* pTransform );

    enum MatrixConvertOrder
    {
        MTX_CVT_SRT,    // Scale * Rotate * Translate;
        MTX_CVT_TRS,    // Trnaslate * Rotate * Scale;
    };

    TsBool SetMatrixConvertOrder(MatrixConvertOrder order)
    {
        m_matrixConvertOrder = order;
        return TS_TRUE;
    }
protected:

    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    TsTransForm* m_pTransform;
    TsMatrix	 m_matrixCash;

    MatrixConvertOrder m_matrixConvertOrder = MTX_CVT_SRT;
};