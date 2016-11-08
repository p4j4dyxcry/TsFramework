#include "../TsGfx.h"

TsCamera::TsCamera() :
m_aspect(16.0f / 9.0f),
m_fov(45),
m_near(0.001f),
m_far(1500),
m_pCameraBuffer(nullptr),
m_pCBufferMemory(nullptr)
{};

TsCamera::~TsCamera()
{
    TsSafeDelete(m_pCameraBuffer);
}

TsBool TsCamera::UpdateForCBuffer(TsDevice* pDevice)
{
    if (m_pCBufferMemory == nullptr)
        return TS_FALSE;

    // ibl test code
#if 0
    static float r = 0;
    static float m = 0;
    if (GetAsyncKeyState(VK_LEFT))
    {
        r = r <= 0 ? 0 : r - 0.1f;
    }
    if (GetAsyncKeyState(VK_RIGHT))
    {
        r = r >= 1 ? 1 : r + 0.1f;
    }

    if (GetAsyncKeyState('Z'))
    {
        m = m <= 0 ? 0 : m - 0.1f;
    }
    if (GetAsyncKeyState('X'))
    {
        m = m >= 1 ? 1 : m + 0.1f;
    }


    m_pCBufferMemory->m_worldCameraPos = m_eye;
    m_pCBufferMemory->m_near = m_near;
    m_pCBufferMemory->m_far = m;
    m_pCBufferMemory->m_fov = r;
#endif
    TsVector3 p = GetWorldPosition();
    m_pCBufferMemory->m_worldCameraPos = TsFloat4(p.x,p.y,p.z,1);
    m_pCBufferMemory->m_near = m_near;
    m_pCBufferMemory->m_far = m_far;
    m_pCBufferMemory->m_fov = m_fov;

    m_pCBufferMemory->m_viewMatrix = GetViewMatrix().Transposed();
    m_pCBufferMemory->m_invViewMatrix = GetViewMatrix().Inversed().Transposed();

    m_pCBufferMemory->m_projMatrix = GetProjMatrix().Transposed();
    m_pCBufferMemory->m_invProjMatrix = GetProjMatrix().Inversed().Transposed();

    m_pCBufferMemory->m_viewProjMatrix = GetViewProjMatrix().Transposed();

    m_pCBufferMemory->m_dumy = 0;

    //call map & unmap
    if (m_pCameraBuffer)
        pDevice->GetDC()->ChangeCBuffer(m_pCameraBuffer, m_pCBufferMemory, sizeof(ViewCBuffer));

    return TS_TRUE;
}

const TsCBuffer* TsCamera::GetCBuffer()const
{
    return m_pCameraBuffer;
}

TsBool TsCamera::CreateCBuffer(TsDevice* pDevice)
{
    TsSafeDelete(m_pCBufferMemory);
    TsSafeDelete(m_pCameraBuffer);

    m_pCBufferMemory = TsNew(ViewCBuffer);

    UpdateForCBuffer(pDevice);

    m_pCameraBuffer = pDevice->CreateCBuffer(m_pCBufferMemory, sizeof(ViewCBuffer));
    if (m_pCameraBuffer == nullptr)
        return TS_FALSE;
    m_pCameraBuffer->SetRegisterIndex(TS_CBUFFER_REGISTER::View);
    m_pCameraBuffer->BindShaderType(TS_SHADER_TYPE::VP_SHADER);

    return TS_TRUE;
}

TsBool TsCamera::Create(TsF32	 aspect ,
                         TsF32	 fov ,
                         TsF32	 _near ,
                         TsF32	 _far )
{
    m_aspect = aspect;
    m_fov = fov;
    m_near = _near;
    m_far = _far;

    return TS_TRUE;
}


TsMatrix TsCamera::GetViewMatrix()const
{
    TsMatrix m = GetWorldMatrix();
    Matrix view;

    TsVector3 lookAtDistance = m_lookAt - GetWorldPosition();

    if (m_isLookAt == TS_FALSE)
        lookAtDistance = TsVector3::zero;

    TsVector3 xAxis , yAxis , zAxis;
    zAxis = TsVector3( m._31 , m._32 , m._33 );
    yAxis = TsVector3( m._21 , m._22 , m._23 );
    xAxis = TsVector3::Cross( yAxis , zAxis );
    yAxis = TsVector3::Cross( zAxis , xAxis );

    view._11 = xAxis.x;
    view._21 = xAxis.y;
    view._31 = xAxis.z;

    view._12 = yAxis.x;
    view._22 = yAxis.y;
    view._32 = yAxis.z;

    view._13 = zAxis.x;
    view._23 = zAxis.y; 
    view._33 = zAxis.z;

    view._41 = -TsVector3::Dot(TsVector3(view._11, view._21, view._31), GetWorldPosition() + lookAtDistance);
    view._42 = -TsVector3::Dot(TsVector3(view._12, view._22, view._32), GetWorldPosition() + lookAtDistance);
    view._43 = -TsVector3::Dot(TsVector3(view._13, view._23, view._33), GetWorldPosition() + lookAtDistance);

    view = view * TsMatrix::CreateTranslate(lookAtDistance);

    return view;
}
TsMatrix TsCamera::GetViewProjMatrix()
{
    return GetViewMatrix() * GetProjMatrix();
}
TsMatrix TsCamera::GetProjMatrix()const
{
    Matrix proj =
        XMMatrixPerspectiveFovLH(
        TsRadian( m_fov ) ,
        m_aspect ,
        m_near ,
        m_far );
    return proj;
}

TsMatrix TsCamera::GetLinearProjMatrix()const
{
    // 2/(r-l)      0             0           0
    // 0            2/(t-b)       0           0
    // 0            0            -1/(zf-zn)   0
    // (l+r)/(l-r)  (t+b)/(b-t)  -zn/(zf-zn)  1

    TsMatrix proj = GetProjMatrix();
    proj._22 /= m_far;
    proj._32 /= m_far;
    return proj;
}

TsBool TsCamera::SetFov( TsF32 fov )
{
    m_fov = fov;
    return TS_TRUE;
}
TsBool TsCamera::SetNearAndFar( TsF32 _near , TsF32 _far )
{
    m_near = _near;
    m_far = _far;
    return TS_TRUE;
}