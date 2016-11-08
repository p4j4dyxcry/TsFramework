#include "../TsGfx.h"

//===================================================================
//  ! Constructor
//===================================================================
TsCamera::TsCamera() :
m_aspect(16.0f / 9.0f),
m_fov(45),
m_near(0.001f),
m_far(1500),
m_pViewCBuffer(nullptr)
{
    memset(&m_bufferResource, 0, sizeof(m_bufferResource));
};

//===================================================================
//  ! Destructor
//===================================================================
TsCamera::~TsCamera()
{
    TsSafeDelete(m_pViewCBuffer);
}

//=========================================================================
//! UpdateForCBuffer
//  �s��̌v�Z�EConstantBuffer���X�V
//=========================================================================
TsBool TsCamera::UpdateForCBuffer(TsDevice* pDevice)
{
    if (m_pViewCBuffer == nullptr)
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
    m_bufferResource.m_worldCameraPos = TsFloat4(p.x, p.y, p.z, 1);
    m_bufferResource.m_near = m_near;
    m_bufferResource.m_far = m_far;
    m_bufferResource.m_fov = m_fov;

    m_bufferResource.m_viewMatrix = GetViewMatrix().Transposed();
    m_bufferResource.m_invViewMatrix = GetViewMatrix().Inversed().Transposed();

    m_bufferResource.m_projMatrix = GetProjMatrix().Transposed();
    m_bufferResource.m_invProjMatrix = GetProjMatrix().Inversed().Transposed();

    m_bufferResource.m_viewProjMatrix = GetViewProjMatrix().Transposed();

    m_bufferResource.m_dumy = 0;

    //call map & unmap
    pDevice->GetDC()->ChangeCBuffer(m_pViewCBuffer, &m_bufferResource, sizeof(ViewCBuffer));

    return TS_TRUE;
}

//=========================================================================
//! GetCBuffer
//  Constant Buffer �̎擾
//=========================================================================
const TsCBuffer* TsCamera::GetCBuffer()const
{
    return m_pViewCBuffer;
}

//=========================================================================
//! CreateBuffer
//  Constant Buffer �̍쐬
//=========================================================================
TsBool TsCamera::CreateCBuffer(TsDevice* pDevice)
{
    TsSafeDelete(m_pViewCBuffer);


    UpdateForCBuffer(pDevice);

    m_pViewCBuffer = pDevice->CreateCBuffer(&m_bufferResource, sizeof(ViewCBuffer));
    if (m_pViewCBuffer == nullptr)
        return TS_FALSE;
    m_pViewCBuffer->SetRegisterIndex(TS_CBUFFER_REGISTER::ViewCB);
    m_pViewCBuffer->BindShaderType(TS_SHADER_TYPE::VP_SHADER);

    return TS_TRUE;
}

//=========================================================================
//! CreateCamera
//  �J�����̍쐬
//=========================================================================
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

//=========================================================================
//! GetViewMatrix
//  View�s��( LookAtLH)�̎擾
//=========================================================================
TsMatrix TsCamera::GetViewMatrix()const
{
    /*
        -Memo
        ����Ă�����e��TsMatrix::CreateLookAt()�Ɠ����B
        �����_��]���������邽�߂ɓƎ��������Ă���B
    */

    TsMatrix m = GetWorldMatrix();  // Camera��Transform���擾
    TsMatrix view;
 
    //=========================================================================
    //! Camera�̃g�����X�t�H�[������ View�s����쐬����
    //  zAxis���S�ɐV���� xAxis �� yAxis�����߂�K�v������
    //=========================================================================
    TsVector3 xAxis , yAxis , zAxis;
    zAxis = TsVector3( m._31 , m._32 , m._33 ); // zAxis�̎Z�o
    yAxis = TsVector3( m._21 , m._22 , m._23 ); // ����yAxis
    xAxis = TsVector3::Cross( yAxis , zAxis );  // xAxis�̎Z�o(Cross�͂��̏��Ԃɂ��邱�ƁB)
    yAxis = TsVector3::Cross( zAxis , xAxis );  // yAxis�̎Z�o(Cross�͂��̏��Ԃɂ��邱�ƁB)
    /*
        -Memo View�s��̓����f�[�^
        _11 xAngle.x            , _12 xAngle.y            , _13 xAngle.z              _14  ,0
        _21 yAngle.x            , _22 yAngle.y            , _23 yAngle.z              _24  ,0
        _31 zAngle.x            , _32 zAngle.y            , _33 zAngle.z              _34  ,0
        _41 -dot( xAngle , eye) , _42 -dot( yAngle , eye) , _43 -dot( zAngle , eye )  _44  ,1    
    */
    
    //=========================================================================
    //! �����_�v�Z���s��
    //=========================================================================
    TsVector3 lookAtDistance = TsVector3::zero;

    if (m_isLookAt == TS_FALSE)
        lookAtDistance = m_lookAt - GetWorldPosition();
    
    view._11 = xAxis.x;
    view._21 = xAxis.y;
    view._31 = xAxis.z;

    view._12 = yAxis.x;
    view._22 = yAxis.y;
    view._32 = yAxis.z;

    view._13 = zAxis.x;
    view._23 = zAxis.y; 
    view._33 = zAxis.z;

    //�����_�ŉ�]�������邽�ߒ����_�ɃJ�������ړ�������B
    TsVector3 diffPos = GetWorldPosition() + lookAtDistance;

    view._41 = -TsVector3::Dot(TsVector3(view._11, view._21, view._31), diffPos);
    view._42 = -TsVector3::Dot(TsVector3(view._12, view._22, view._32), diffPos);
    view._43 = -TsVector3::Dot(TsVector3(view._13, view._23, view._33), diffPos);

    //�����_�܂ňړ�������̓J������߂��B
    view = view * TsMatrix::CreateTranslate(lookAtDistance);

    return view;
}

//=========================================================================
//! GetViewProjeMatrix
//  View�s�� * Projection�s��̐ς��擾
//=========================================================================
TsMatrix TsCamera::GetViewProjMatrix()
{
    return GetViewMatrix() * GetProjMatrix();
}

//=========================================================================
//! GetProjeMatrix
//  Projection�s����v�Z���擾����B
//=========================================================================
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

//=========================================================================
//! GetLinearProjeMatrix
//  near - far �����`�����ꂽProjection�s����v�Z���擾����B
//=========================================================================
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

//=========================================================================
//! SetFov
//  ����p��ݒ肷��(radian)
//=========================================================================
TsBool TsCamera::SetFov( TsF32 fov )
{
    m_fov = fov;
    return TS_TRUE;
}

//=========================================================================
//! SetNearAndFar
//  �j�A�v���[���E�t�@�[�v���[����ݒ肷��
//=========================================================================
TsBool TsCamera::SetNearAndFar( TsF32 _near , TsF32 _far )
{
    m_near = _near;
    m_far = _far;
    return TS_TRUE;
}

//=========================================================================
//  !SetLockAt
//
//  �����_��ݒ肵�܂��B
//  �����_���ݒ肳�ꂽ�J�����͒����_����Ɍ����悤�ɂȂ�܂��B
//  �A���A���̃��\�b�h���Ăяo���O��UsingLookAtTarget(TS_TRUE)��
//  �ݒ肵�Ă����K�v������܂��B
//=========================================================================
TsBool TsCamera::SetLockAt(TsVector3 p)
{
    m_lookAt = p;
    return TS_TRUE;
}

//=========================================================================
//  !UsingLookAtTarget
//  �����_�̎g�p���ۂ�ݒ肵�܂��B
//=========================================================================
TsBool TsCamera::UsingLookAtTarget(TsBool flag)
{
    m_isLookAt = flag;
    return TS_TRUE;
}