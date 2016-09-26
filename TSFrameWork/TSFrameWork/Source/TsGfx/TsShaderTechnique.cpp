#include "TsGfx.h"

TsShaderTechnique::TsShaderTechnique()
{
	m_pCBuffers.reserve( 32 );
}
TsShaderTechnique::~TsShaderTechnique()
{
	for each( auto var  in m_pCBuffers )
		TsSafeDelete( var );
	m_pCBuffers.clear();
}

TsBool TsShaderTechnique::Initialize( TsDevice* pDevice, TsShaderEffect* pShaderEffect)
{
	( void )pDevice;
	( void )pShaderEffect;
	return TS_FALSE;
}
TsBool TsShaderTechnique::UpdateCBuffer( TsDeviceContext * pDC )
{
	( void )pDC;
	return TS_FALSE;
}
TsBool TsShaderTechnique::ApplyCBuffer( TsDeviceContext * pDC )
{
	( void )pDC;
	return TS_FALSE;
}

