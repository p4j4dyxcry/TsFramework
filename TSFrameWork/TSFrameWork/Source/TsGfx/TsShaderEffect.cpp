#include "TsGfx.h"


TsBool	TsShaderEffect::SetVertexShader( TsVertexShader* shader)
{
	m_vertexShader = shader;
	return TS_TRUE;
}
TsBool	TsShaderEffect::SetPixelShader( TsPixelShader* shader)
{
	m_pixelShader = shader;
	return TS_TRUE;
}
TsBool	TsShaderEffect::SetGeometryShader( TsGeometryShader* shader )
{
	m_geometoryShader = shader;
	return TS_TRUE;
}
TsBool	TsShaderEffect::SetHullShader( TsHullShader* shader )
{
	m_hullShader = shader;
	return TS_TRUE;
}
TsBool	TsShaderEffect::SetDomainShader( TsDomainShader* shader )
{
	m_domainShader = shader;
	return TS_TRUE;
}
TsBool	TsShaderEffect::SetComputeShader( TsComputeShader* shader )
{
	m_computeShader = shader;
	return TS_TRUE;
}

ID3D11InputLayout* TsShaderEffect::GetInputLayout()
{
	if( m_vertexShader == nullptr )
		return nullptr;
	return m_vertexShader->GetInputLayout();
}