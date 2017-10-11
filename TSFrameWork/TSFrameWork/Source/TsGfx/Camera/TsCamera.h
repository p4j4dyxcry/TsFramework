//!*******************************************************
//! TsCamera.h
//!
//! Basic Camera Class.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsCamera : public TsNameObject,
                 public IHasTransform
{
public:

    // Constructor
    TsCamera();

    // Destructor
    virtual ~TsCamera();
    //==========================================================
    //  !Create Camera
    //    
    // @pram aspect viewport Aspect . for example 16.0f / 9.0f
    // @pram fov    view Radian Angle
    // @pram near   frustom near
    // @pram far    frustom far
    TsBool Create( TsF32	 aspect,
                   TsF32	 fov,
                   TsF32	 near,
                   TsF32	 far);

    //==========================================================
    //  !Get Parameters
    //==========================================================
    TsMatrix GetProjMatrix()const;
    TsMatrix GetLinearProjMatrix()const;
    TsMatrix GetViewMatrix()const;
    TsMatrix GetViewProjMatrix();// const;

    //==========================================================
    //  !Create CBuffer
    //  作成したCBufferはTsShaderUT.hで定義されているレジスタにバインド    
    //  されます。
    // @pram device 
    TsBool	 CreateCBuffer( TsDevice* );

    //==========================================================
    //  !UpdateForCBuffer
    //  更新されたパラメータを元に行列を作成し、CBufferを更新します。
    TsBool	 UpdateForCBuffer( TsDevice* );

    //==========================================================
    //  !GetCBuffer
    //  CBuffer　を取得します。
    const CBufferBase* GetCBuffer()const;

    //==========================================================
    //  !SetFov
    //  Fov(視野角)を設定します(Radian)
    TsBool SetFov( TsF32 fov );

    //==========================================================
    //  !Set Near And Far
    //  ニアプレーンとファープレーンを設定します。
    TsBool SetNearAndFar( TsF32 _near , TsF32 _far );

    //==========================================================
    //  !GetFov
    //  Fov(視野角)を取得します(Radian)。
    TsF32 GetFov(){ return m_fov; }

    //==========================================================
    //  !GetNear
    //  ニアプレーンを取得しますを取得します。
    TsF32 GetNear(){ return m_near; }

    //==========================================================
    //  !GetFar
    //  ファープレーンを取得しますを取得します。
    TsF32 GetFar(){ return m_far; }

    //==========================================================
    //  !GetAspect
    //  設定されているアスペクトト比の除算結果を取得します。
    TsF32 GetAspect(){ return m_aspect; }

    TsVector3 GetLookAt()const{ return m_lookAt; }

    //==========================================================
    //  !SetLockAt
    //  注視点を設定します。
    //  注視点が設定されたカメラは注視点を常に向くようになります。
    //  但し、このメソッドを呼び出す前にUsingLookAtTarget(TS_TRUE)を
    //  設定しておく必要があります。
    TsBool SetLockAt(TsVector3 p);

    //==========================================================
    //  !UsingLookAtTarget
    //  注視点の使用を可否を設定します。
    TsBool UsingLookAtTarget(TsBool flag);
public:

    //==========================================================
    //  GPUに転送するデータ構造を定義
    //==========================================================
    struct ViewCBuffer
    {
        TsMatrix m_viewMatrix;
        TsMatrix m_projMatrix;
        TsMatrix m_invViewMatrix;
        TsMatrix m_invProjMatrix;
        TsMatrix m_viewProjMatrix;

        TsFloat4 m_worldCameraPos;
        //TsFloat4 m_projRaito;

        TsF32    m_near;
        TsF32    m_far;
        TsF32    m_fov;
        TsF32    m_dumy;
    };

protected:

    TsF32 m_aspect; 
    TsF32 m_fov;

    TsF32 m_near;
    TsF32 m_far;

    CBufferBase*    m_pViewCBuffer;
    ViewCBuffer     m_bufferResource;

    //注視点設定用
    TsVector3       m_lookAt;
    TsBool          m_isLookAt = TS_TRUE;
};