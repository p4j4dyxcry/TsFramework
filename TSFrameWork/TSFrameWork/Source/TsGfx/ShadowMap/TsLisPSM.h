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
    TsBool SetViewDir(const TsVector3& value);
    TsBool SetLightDir(const TsVector3& value);
    TsBool SetEyeProjection(const TsMatrix& value);
    TsBool SetNearClip(const TsF32 value);
    TsBool UpdateShadowMatrix();

    TsMatrix GetLVMatrix();
    TsMatrix GetLPMatrix();
    TsMatrix GetLVPMatrix();

private:
    //----------------------------------------------------------
    // Define
    //----------------------------------------------------------
    class PointList
    {
    private:
        TsVector<TsVector3> m_list;
    public:
        PointList();
        TsInt GetSize()const;
        TsBool Add( TsVector3& value );
        TsBool Clear();
        TsBool Transform( const TsMatrix& matrix );
        TsBool ComputeAABB( TsVector3& min , TsVector3& max )const;

        PointList & operator = ( PointList& value );
        TsVector3& operator[] ( TsInt index );
    };

    //----------------------------------------------------------
    // private method
    //----------------------------------------------------------
    TsBool ComputeLisPSM();
    TsBool ComputeShadowMap();
    TsBool ComputeViewFrustum(TsMatrix& viewProj);
    TsVector3 ComputeUpVector(TsVector3& viewDir, TsVector3 lightDir);
    TsMatrix GetUnitCubeClipMatrix(TsVector3& min, TsVector3& max);
    TsMatrix GetPerspective(TsF32 nearDist, TsF32 farDist);


    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    PointList m_pointList;

    TsVector3 m_eyePostion;
    TsVector3 m_viewDir;
    TsVector3 m_lightDir;

    TsMatrix m_viewMatrix;
    TsMatrix m_viewProjectionMatrix;

    TsMatrix m_lightViewMatrix;
    TsMatrix m_lightProjectionMatrix;
    TsMatrix m_lVPMatrix;

    TsF32  m_near;
};