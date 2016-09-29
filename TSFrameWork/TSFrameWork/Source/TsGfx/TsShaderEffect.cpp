#include "TsGfx.h"

TS_INSTANTIATE_NAME_OBJ_LIST(TsShaderEffect);

TsBool TsShaderEffect::LoadPackageFromXml( TsDevice* pDev , const TsString& name )
{
	TsXML xmlShaderPass;
	xmlShaderPass.LoadXML( TSUT::Resource::GetShaderPackageDirectory() + name );

	TsString passName = xmlShaderPass.FindFirst( "name" )->GetAttribute( "name" )->GetStringValue();
	SetName( passName );

	TsString ShaderDir = TSUT::Resource::GetCSODirectory();

	TsString ShaderName;
	TsXMLElement* xmlElement = xmlShaderPass.FindFirst( "Vertex" );

	if( xmlElement != nullptr )
	{
		ShaderName = xmlElement->GetAttribute( "Shader" )->GetStringValue() + ".cso";
		TsVertexShader* pVS = TsNew( TsVertexShader );
		pVS->LoadFromCSO( pDev->GetDevD3D() , ( ShaderDir + ShaderName ).c_str() );

		SetVertexShader( pVS );
	}

	xmlElement = xmlShaderPass.FindFirst( "Pixel" );

	if( xmlElement != nullptr )
	{
		ShaderName = xmlElement->GetAttribute( "Shader" )->GetStringValue() + ".cso";
		TsPixelShader* pPS = TsNew( TsPixelShader );
		pPS->LoadFromCSO( pDev->GetDevD3D() , ( ShaderDir + ShaderName ).c_str() );

		SetPixelShader( pPS );
	}
	return TS_TRUE;
}
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
