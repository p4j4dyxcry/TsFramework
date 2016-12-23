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
    TsBool SetViewMatrix( const TsMatrix& viewMatrix );
    TsBool SetLightDir(const TsVector3& value);
    TsBool SetProjection(const TsMatrix& value);
    TsBool SetSceneBoundingBox(const TsAABB3D& aabb);
    TsBool UpdateShadowMatrix();

    TsMatrix GetLVMatrix()const;
    TsMatrix GetLPMatrix()const;
    TsMatrix GetLVPMatrix()const;

private:
    //----------------------------------------------------------
    // private method
    //----------------------------------------------------------
    TsBool ComputeLisPSM();
    TsBool ComputeUSM();

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

    TsAABB3D m_sceneBoundingBox;
};