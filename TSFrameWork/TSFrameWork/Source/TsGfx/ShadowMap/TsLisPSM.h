//**********************************************************
//! TsLisPSM.h
//! ライススペースパースペクティブシャドウマトリクスを計算する.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

class TsComputeLisPSM
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsComputeLisPSM();

    TsBool SetEyePos(const TsVector3& value);
    TsBool SetEyeViewMatrix( const TsMatrix& viewMatrix );
    TsBool SetLightDir(const TsVector3& value);
    TsBool SetProjection(const TsMatrix& value);
    TsBool UpdateShadowMatrix();

    TsMatrix GetLVMatrix();
    TsMatrix GetLPMatrix();
    TsMatrix GetLVPMatrix();

private:
    //----------------------------------------------------------
    // private method
    //----------------------------------------------------------
    TsBool ComputeLisPSM();

    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    TsVector3 m_eyePostion;
    TsVector3 m_viewDir;
    TsVector3 m_lightDir;

    TsMatrix m_viewMatrix;
    TsMatrix m_viewProjectionMatrix;

    TsMatrix m_lightViewMatrix;
    TsMatrix m_lightProjectionMatrix;
    TsMatrix m_lVPMatrix;

    TsF32  m_near;
    TsF32  m_far;
};