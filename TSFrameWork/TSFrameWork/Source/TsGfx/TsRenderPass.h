//!*******************************************************
//! TsRenderPass.h
//!
//! The class is shader flow.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsRenderPass :public TsNameObject ,
                    public ITsStaticNameObjectList<TsRenderPass>
{
public:
    TsRenderPass();
    virtual ~TsRenderPass();

    virtual TsBool Begin( TsDeviceContext* );
    virtual TsBool Render(TsDrawQueue* pQue , TsDeviceContext* );
    virtual TsBool End( TsDeviceContext* );

    virtual TsBool SetShader( TsShaderEffect* );

    virtual TsBool SetInputSlot( TsInt index , TsRenderTarget* );
    virtual TsBool SetOutputSlot( TsInt index , TsRenderTarget * rtv );
    virtual TsBool SetDepthSlot( TsDepthStencilView* pDepth );

    virtual TsRenderTarget* GetInputSlot( TsInt index );
    virtual TsRenderTarget* GetOutputSlot( TsInt index = 0 );

    virtual TsBool LoadShaderFromXMLElement( TsDevice* pDev , TsXMLElement * pElement );
    virtual TsBool LoadIOSlotFromXMLElement( TsDevice* pDev , TsXMLElement * pElement );
     
protected:
    TsBool ApplyRTV( TsDeviceContext* pDC );
protected:
    TsRenderTarget*         m_pInputSlot[TsDeviceContext::MAX_RTs];
    TsRenderTarget*         m_pOutputSlot[TsDeviceContext::MAX_RTs];
    TsDepthStencilView*  m_pDepthStencilView;
    TsShaderEffect*         m_pShader;

    TsRasterizerState*      m_pRasterizerState;
    TsDepthStencilState* m_pDepthStencilState;
};