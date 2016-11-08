#pragma once

class TsCamera : public TsNameObject,
                 public IHasTransform
{
public:
    
    static const TsInt MainCameraCbufferRegisterIndex = 7;

    TsCamera();
    virtual ~TsCamera();
    TsBool Create( TsF32	 aspect,
                   TsF32	 fov,
                   TsF32	 near,
                   TsF32	 far);

    TsMatrix GetProjMatrix()const;
    TsMatrix GetLinearProjMatrix()const;
    TsMatrix GetViewMatrix()const;
    TsMatrix GetViewProjMatrix();// const;
    TsBool	 CreateCBuffer( TsDevice* );
    TsBool	 UpdateForCBuffer( TsDevice* );
    const TsCBuffer* GetCBuffer()const;

    TsBool SetFov( TsF32 fov );
    TsBool SetNearAndFar( TsF32 _near , TsF32 _far );
    TsF32 GetFov(){ return m_fov; }
    TsF32 GetNear(){ return m_near; }
    TsF32 GetFar(){ return m_far; }
    TsF32 GetAspect(){ return m_aspect; }
public:
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
    TsBool SetLockAt( TsVector3 p )
    {
        m_lookAt = p;
        return 1;
    }

protected:

    TsF32 m_aspect; 
    TsF32 m_fov;

    TsF32 m_near;
    TsF32 m_far;

    TsCBuffer*      m_pCameraBuffer;
    ViewCBuffer*    m_pCBufferMemory;

    //サテライト用
    TsVector3       m_lookAt;
    TsBool          m_isLookAt = TS_TRUE;
};