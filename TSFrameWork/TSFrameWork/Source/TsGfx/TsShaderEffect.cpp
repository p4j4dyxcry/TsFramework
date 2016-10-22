#include "TsGfx.h"

TS_INSTANTIATE_NAME_OBJ_LIST(TsShaderEffect);

TsBool TsShaderEffect::LoadPackageFromXml( TsDevice* pDev , const TsString& name )
{
    TsXML xmlShaderPass;
    xmlShaderPass.LoadXML( TSUT::Resource::GetShaderPackageDirectory() + name );

    TsString passName = xmlShaderPass.FindFirst( "Name" )->GetAttribute( "Name" )->GetStringValue();
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

    xmlElement = xmlShaderPass.FindFirst( "DepthStencil" );
    m_pDepthStencilState = TsNew( TsDepthStencilState );
    if( xmlElement != nullptr )
    {
        TsXMLAttribute* pAttr = xmlElement->GetAttribute( "ZEnable" );
        if( pAttr )
        {
            TsBool zEnable = pAttr->GetBoolValue();
            m_pDepthStencilState->SetZEnable( zEnable );
        }

        pAttr = xmlElement->GetAttribute( "ZWriteEnable" );
        if( pAttr )
        {
            TsBool zWriteEnable = pAttr->GetBoolValue();
            m_pDepthStencilState->SetZWriteEnable( zWriteEnable );
        }

        pAttr = xmlElement->GetAttribute( "Func" );
        if( pAttr )
        {
            TsString depthTestFunc = pAttr->GetStringValue();
            m_pDepthStencilState->SetDepthTestFuncByString( depthTestFunc );
        }
    }
    m_pDepthStencilState->CreateDepthStencil( pDev );

    xmlElement = xmlShaderPass.FindFirst( "Rasterize" );
    m_pRasterizerState= TsNew( TsRasterizerState );
    if( xmlElement != nullptr )
    {
        TsXMLAttribute* pAttr = xmlElement->GetAttribute( "FillMode" );
        if( pAttr )
        {
            TsString fillmode = pAttr->GetStringValue();
            m_pRasterizerState->SetFillModeByString( fillmode );
        }

        pAttr = xmlElement->GetAttribute( "CullMode" );
        if( pAttr )
        {
            TsString cullmode = pAttr->GetStringValue();
            m_pRasterizerState->SetCullModeByString( cullmode );
        }

        pAttr = xmlElement->GetAttribute( "AntiAlias" );
        if( pAttr )
        {
            TsBool antiailias = pAttr->GetBoolValue();
            m_pRasterizerState->SetAntiAliasMode( antiailias );
        }
    }

    m_pRasterizerState->CreateRasterizerState( pDev );
    return TS_TRUE;
}
TsBool	TsShaderEffect::SetVertexShader( TsVertexShader* shader)
{
    m_vertexShader = shader;
    return TS_TRUE;
}
TsBool	TsShaderEffect::SetPixelShader( TsPixelShader* shader)
{
    m_pPixelShader = shader;
    return TS_TRUE;
}
TsBool	TsShaderEffect::SetGeometryShader( TsGeometryShader* shader )
{
    m_pGeometoryShader = shader;
    return TS_TRUE;
}
TsBool	TsShaderEffect::SetHullShader( TsHullShader* shader )
{
    m_pHullShader = shader;
    return TS_TRUE;
}
TsBool	TsShaderEffect::SetDomainShader( TsDomainShader* shader )
{
    m_pDomainShader = shader;
    return TS_TRUE;
}
TsBool	TsShaderEffect::SetComputeShader( TsComputeShader* shader )
{
    m_pComputeShader = shader;
    return TS_TRUE;
}

ID3D11InputLayout* TsShaderEffect::GetInputLayout()
{
    if( m_vertexShader == nullptr )
        return nullptr;
    return m_vertexShader->GetInputLayout();
}
