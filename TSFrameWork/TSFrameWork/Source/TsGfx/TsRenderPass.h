//!*******************************************************
//!	TsRenderPass.h
//!
//!	The class is shader flow.
//!
//!	© 2016 Yuki Tsuneyama
#pragma once

class TsRenderPass :public TsNameObject ,
					public ITsStaticNameObjectList<TsRenderPass>
{
public:
	TsRenderPass();
	virtual ~TsRenderPass();

	TsBool Begin( TsDeviceContext* );
	TsBool Render( TsDeviceContext* );
	TsBool End	( TsDeviceContext* );

	TsBool SetShader( TsShaderEffect* );
	TsBool ZWrite( TsBool );
	TsBool ZEnable( TsBool );
	TsBool CullMode( D3D11_CULL_MODE );
	TsBool SetInputSlot( TsInt index , TsRenderTarget* );
	TsBool SetOutputSlot( TsInt index, TsRenderTarget * rtv );
	TsBool SetDepthSlot( TsDepthStencil* pDepth );
	TsBool SetDrawQueue( TsDrawQueue * queue );
	TsRenderTarget* GetInputSlot(TsInt index);
	TsRenderTarget* GetOutputSlot(TsInt index = 0);

protected:
	TsDrawQueue*	m_pdrawQueue;
	TsRenderTarget* m_pInputSlot[TsDeviceContext::MAX_RTs];
	TsRenderTarget* m_pOutputSlot[TsDeviceContext::MAX_RTs];
	TsDepthStencil*  m_pDepthStencil;
	TsShaderEffect*  m_pShader;

	TsBool m_zEnable;
	TsBool m_zWriteEnable;
	D3D11_CULL_MODE m_cullMode;
};