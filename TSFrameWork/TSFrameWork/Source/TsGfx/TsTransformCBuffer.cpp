#include "TsGfx.h"

TsTransformCBuffer::TsTransformCBuffer()
	:m_pTransform( nullptr )
{}

TsBool TsTransformCBuffer::UpdateCBuffer( TsDeviceContext * pContext )
{
	if( m_pTransform )
	{
		TsMatrix mtxView = pContext->GetMainCamera()->GetViewMatrix();
		TsMatrix mtxProj = pContext->GetMainCamera()->GetProjMatrix();
		TsMatrix mtxVP	 = pContext->GetMainCamera()->GetViewProjMatrix();

		m_matrixCBuffer.m_MtxWorld		= m_pTransform->ToWorldMatrix().Transposed();
		m_matrixCBuffer.m_MtxInvWorld	= m_pTransform->ToWorldMatrix().Inversed().Transposed();

		pContext->ChangeCBuffer( this , &m_matrixCBuffer , sizeof( m_matrixCBuffer ) );
	}
	else
	{
		return TS_FALSE;
	}

	return TS_TRUE;

}
TsBool TsTransformCBuffer::ApplyCBuffer( TsDeviceContext * pContext )
{
	pContext->SetCBuffer( this );
	return TS_TRUE;
}
TsBool TsTransformCBuffer::CreateTransformCBuffer( TsDevice* pDev )
{
	size_t sz = sizeof( m_matrixCBuffer );

	ID3D11Buffer* buffer = pDev->CreateBuffer( &m_matrixCBuffer ,
												sz ,
												D3D11_CPU_ACCESS_WRITE ,
												D3D11_BIND_CONSTANT_BUFFER );


	SetD3DBufferAndSize( buffer , sz );
	SetRegisterIndex( 0 );
	BindShaderType( TS_SHADER_TYPE::VERTEX_SHADER );

	return TS_TRUE;
}

TsBool TsTransformCBuffer::SetTransform( TsTransForm* pTransform )
{
	m_pTransform = pTransform;
	return TS_TRUE;
}