#include "TsGfx.h"

TsRenderPass::TsRenderPass():
m_cullMode(D3D11_CULL_BACK),
m_pShader(nullptr),
m_zEnable(TS_TRUE),
m_zWriteEnable(TS_TRUE)
{
	for( int i = 0; i < TsDeviceContext::MAX_RTs; ++i )
	{
		m_pInputSlot[i] = nullptr;
		m_pOutputSlot[i] = nullptr;
	}
}
TsRenderPass::~TsRenderPass()
{

}

TsBool TsRenderPass::Begin( TsDeviceContext* pDc )
{
	//! set render target
	for( int i = 0; i < TsDeviceContext::MAX_RTs; ++i )
		pDc->SetRT( i , m_pOutputSlot[i] );

	//! set shader
	pDc->SetShader( m_pShader );

	//! set inputlayout
	pDc->ApplyInputLayout();

	//! draw queue begin
	m_pdrawQueue->Begin( pDc , m_zEnable , m_zWriteEnable , m_cullMode );
	return TS_TRUE;
}
TsBool TsRenderPass::End( TsDeviceContext* context)
{
	( void )context;
	m_pdrawQueue->End();
	//todo
	return TS_TRUE;
}
TsBool TsRenderPass::SetInputSlot( TsInt index , TsRenderTarget *rtv)
{
	if( abs( index ) >= TsDeviceContext::MAX_RTs )
		return TS_FALSE;
	m_pInputSlot[index] = rtv;

	return TS_TRUE;
}

TsBool TsRenderPass::SetOutputSlot(TsInt index, TsRenderTarget *rtv )
{
	if( abs( index ) >= TsDeviceContext::MAX_RTs )
		return TS_FALSE;
	m_pOutputSlot[index] = rtv;

	return TS_TRUE;
}

//入力バッファを取得
TsRenderTarget* TsRenderPass::GetInputSlot( TsInt index )
{
	return m_pInputSlot[ index ];
}

//出力バッファを取得
TsRenderTarget* TsRenderPass::GetOutputSlot( TsInt index /* = 0 */)
{
	return m_pOutputSlot[index];
}

TsBool TsRenderPass::SetShader( TsShaderEffect* se )
{
	m_pShader = se;
	return TS_TRUE;
}
TsBool TsRenderPass::ZWrite( TsBool flg )
{
	m_zWriteEnable = flg;
	return TS_TRUE;
}
TsBool TsRenderPass::ZEnable( TsBool flg)
{
	m_zEnable = flg;
	return TS_TRUE;
}
TsBool TsRenderPass::CullMode( D3D11_CULL_MODE mode)
{
	m_cullMode = mode;
	return TS_TRUE;
}

TsBool TsRenderPass::SetDrawQueue( TsDrawQueue * queue )
{
	m_pdrawQueue = queue;
	return TS_TRUE;
} 

TsBool TsRenderPass::LoadShaderFromXML(TsDevice* pDev,const TsString& name)
{
	TsXML xmlShaderPass;
	xmlShaderPass.LoadXML(TSUT::Resource::GetShaderPassDirectory() + name);

	TsString passName = xmlShaderPass.FindFirst("name")->GetAttribute("name")->GetStringValue();
	SetName(passName);

	TsString ShaderDir = TSUT::Resource::GetCSODirectory();

	m_pShader = TsNew(TsShaderEffect);

	TsString ShaderName;
	TsXMLElement* xmlElement = xmlShaderPass.FindFirst("Vertex");
	
	if (xmlElement != nullptr)
	{
		ShaderName = xmlElement->GetAttribute("Shader")->GetStringValue() + ".cso";
		TsVertexShader* pVS = TsNew(TsVertexShader);
		pVS->LoadFromCSO(pDev->GetDevD3D(), (ShaderDir + ShaderName).c_str());

		m_pShader->SetVertexShader(pVS);
	}

	xmlElement = xmlShaderPass.FindFirst("Pixel");

	if (xmlElement != nullptr)
	{
		ShaderName = xmlElement->GetAttribute("Shader")->GetStringValue() + ".cso";
		TsPixelShader* pPS = TsNew(TsPixelShader);
		pPS->LoadFromCSO(pDev->GetDevD3D(), (ShaderDir + ShaderName).c_str());

		m_pShader->SetPixelShader(pPS);
	}
	return TS_TRUE;
}