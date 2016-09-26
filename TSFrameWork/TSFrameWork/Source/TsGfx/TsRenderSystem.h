//!*******************************************************
//!	TsRenderSystem.h
//!
//!	
//!
//!	©	2016 Yuki Tsuneyama

class TsRenderSystem : TsNameObject
{
public:

	TsRenderSystem();
	virtual ~TsRenderSystem();
	virtual TsBool CallRender( TsDeviceContext* pDC );

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
		PRI_RENDERER,
		DEFAULT_RENDERER,
		POST_RENDERER,
	};
	TsBool SetDrawQue(TsDrawQueue* pDrawQue, TARGET_FLOW = TARGET_FLOW::DEFAULT_RENDERER);
	TsBool SetShaderFlow(TsRenderFlow* pFlow, TARGET_FLOW = TARGET_FLOW::DEFAULT_RENDERER);

	TsTexture* FindTextureResource(const TsString & name);
	TsBool LoadShaderResourceFromXML(TsDevice* pDev, const TsString&);

protected:



	//todo : shadowmap class 
	TsVector<TsLight*>		m_lights;
	TsVector<TsTexture*>	m_rtvList;
	TsVector<TsDrawQueue*>	m_drawque;

	FlowAndQue m_PriFlowAndQue;
	FlowAndQue m_FlowAndQue;
	FlowAndQue m_PostFlowAndQue;

};