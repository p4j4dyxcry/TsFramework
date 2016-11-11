//!*******************************************************
//! TsRenderSystem.h
//!
//! Render System
//!
//! © 2016 Yuki Tsuneyama

class TsRenderSystem : TsNameObject
{
public:
    //----------------------------------------------------------
    // Define
    //----------------------------------------------------------
    struct FlowAndQue
    {
        TsRenderFlow* pFlow = nullptr;
        TsDrawQueue*  pQue = nullptr;

        TsBool Render(TsDeviceContext* pDC)
        {
            if (!pDC || !pQue || !pFlow)
                return TS_FALSE;

            pFlow->Render(pDC, pQue);

            return TS_TRUE;
        }
    };
    enum class TARGET_FLOW
    {
        PRE_RENDERER,
        DEFAULT_RENDERER,
        POST_RENDERER,
    };

    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsRenderSystem();
    virtual ~TsRenderSystem();
    virtual TsBool CallRender( TsDeviceContext* pDC );


    TsBool SetDrawQue(TsDrawQueue* pDrawQue, TARGET_FLOW = TARGET_FLOW::DEFAULT_RENDERER);
    TsBool SetShaderFlow(TsRenderFlow* pFlow, TARGET_FLOW = TARGET_FLOW::DEFAULT_RENDERER);

    TsTexture* FindTextureResource(const TsString & name);
    TsBool LoadShaderResourceFromXML( TsDevice* pDev , const TsString& );
    TsBool LoadRenderSystemFromXML( TsDevice* pDev , const TsString& );
protected:

    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------

    //todo : shadowmap class 
    TsVector<TsLight*>          m_lights;
    TsVector<TsTexture*>        m_shaderResourceList;
    TsVector<TsDrawQueue*>      m_drawque;

    FlowAndQue                  m_PreFlowAndQue;
    FlowAndQue                  m_FlowAndQue;
    FlowAndQue                  m_PostFlowAndQue;

};