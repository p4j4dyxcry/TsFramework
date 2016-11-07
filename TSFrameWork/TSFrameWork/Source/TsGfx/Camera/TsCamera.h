#pragma once

class TsCamera : public TsNameObject
{
public:
    
    static const TsInt MainCameraCbufferRegisterIndex = 7;

    TsCamera();
    virtual ~TsCamera();
    TsBool Create( TsVector3 eye , 
                   TsVector3 up  ,
                   TsVector3 at  ,
                   TsF32	 aspect,
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

    TsBool SetEyePosition( TsVector3 eye );
    TsBool SetUpVector( TsVector3 up );
    TsBool SetLookAtVector( TsVector3 at );
    TsBool SetFov( TsF32 fov );
    TsBool SetNearAndFar( TsF32 _near , TsF32 _far );
    const TsVector3& GetEyePos()const{ return m_eye; }
    const TsVector3& GetLockAtPos()const{ return m_at; }
    const TsVector3& GetUpVector()const{ return m_up; }
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

protected:
    TsVector3 m_eye;
    TsVector3 m_up;
    TsVector3 m_at;

    TsF32 m_aspect; 
    TsF32 m_fov;

    TsF32 m_near;
    TsF32 m_far;

    TsCBuffer*      m_pCameraBuffer;
    ViewCBuffer*    m_pCBufferMemory;
};