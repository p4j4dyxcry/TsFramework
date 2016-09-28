#include "TsGfx.h"

TsRenderSystem::TsRenderSystem()
{

}
TsRenderSystem::~TsRenderSystem()
{

}
TsBool TsRenderSystem::CallRender(TsDeviceContext* pDC)
{
	m_PriFlowAndQue.Render(pDC);
	m_FlowAndQue.Render(pDC);
	m_PostFlowAndQue.Render(pDC);

	return TS_TRUE;
}

TsBool TsRenderSystem::SetDrawQue(TsDrawQueue* pDrawQue, TARGET_FLOW targetFlow )
{
	switch (targetFlow)
	{
	case TARGET_FLOW::PRI_RENDERER	    :m_PriFlowAndQue.pQue = pDrawQue; break;
	case TARGET_FLOW::DEFAULT_RENDERER	:m_FlowAndQue.pQue = pDrawQue; break;
	case TARGET_FLOW::POST_RENDERER		:m_PostFlowAndQue.pQue = pDrawQue; break;
	default:return TS_FALSE;
	}
	return TS_TRUE;
}
TsBool TsRenderSystem::SetShaderFlow(TsRenderFlow* pFlow, TARGET_FLOW targetFlow )
{
	switch (targetFlow)
	{
	case TARGET_FLOW::PRI_RENDERER:m_PriFlowAndQue.pFlow = pFlow; break;
	case TARGET_FLOW::DEFAULT_RENDERER:m_FlowAndQue.pFlow = pFlow; break;
	case TARGET_FLOW::POST_RENDERER:m_PostFlowAndQue.pFlow = pFlow; break;
	default:return TS_FALSE;
	}
	return TS_TRUE;
}

TsTexture* TsRenderSystem::FindTextureResource(const TsString& name)
{
	TS_HASH hash = TSUT::StringToHash(name);
	for each(auto it in m_rtvList)
	{
		if (hash == it->GetHashCode())
			return it;
	}
	return nullptr;
}

TsBool TsRenderSystem::LoadShaderResourceFromXML(TsDevice* pDev, const TsString& name)
{
	TsXML xml;
	xml.LoadXML(TSUT::Resource::GetShaderResourceDirectory() + name);

	auto elm = xml.GetRootNode()->GetFirstChild();

	while (elm != nullptr)
	{
		if (elm->GetName() == "Texture")
		{
			TsTexture * pTex = TsNew(TsTexture2D);
			TsString name = elm->GetAttribute("Name")->GetStringValue();
			TsString path = elm->GetAttribute("Path")->GetStringValue();
			auto srv = TsDirectXTex::LoadFromFile(pDev->GetDevD3D(), path.c_str());
			pTex->SetName(name);
			pTex->SetSRV(srv);

			m_rtvList.push_back(pTex);
		}
		if (elm->GetName() == "RenderTarget")
		{
			TsString name = elm->GetAttribute("Name")->GetStringValue();
			TsString format = elm->GetAttribute("Format")->GetStringValue();
			TsInt2   size = elm->GetAttribute("Size")->GetInt2Value();

			if( size.x < 0 )
				size = pDev->GetDC()->GetScreenRTV()->GetRTVSize();
			auto rtv = TsRenderTarget::CreateRTV(name, *pDev, size.x, size.y,TSShaderUT::ComvertTextureFormat(format));

			m_rtvList.push_back(rtv);
		}
		elm = elm->Next();
		
	}
	return TS_TRUE;
}