#include "TsGfx.h"

TsCamera::TsCamera() :
m_eye(TsVector3(0, 0, 3)),
m_up(TsVector3(0, 1, 0)),
m_at(TsVector3(0, 0, 0)),
m_aspect(16.0f / 9.0f),
m_fov(10),
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

	m_pCBufferMemory->m_worldCameraPos = m_eye;
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
	m_pCameraBuffer->SetRegisterIndex(MainCameraCbufferRegisterIndex);
	m_pCameraBuffer->BindShaderType(TS_SHADER_TYPE::VP_SHADER);

	return TS_TRUE;
}

TsBool TsCamera::Create( TsVector3 eye ,
						 TsVector3 up ,
						 TsVector3 at ,
						 TsF32	 aspect ,
						 TsF32	 fov ,
						 TsF32	 _near ,
						 TsF32	 _far )
{

	m_eye = eye;
	m_up = up;
	m_at = at;
	
	//todo 

	return TS_TRUE;
}


TsMatrix TsCamera::GetViewMatrix()const
{
	Matrix view =
		XMMatrixLookAtLH( 
		m_eye.ToXMVECTOR() ,
		m_at.ToXMVECTOR() ,
		m_up.ToXMVECTOR() );
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