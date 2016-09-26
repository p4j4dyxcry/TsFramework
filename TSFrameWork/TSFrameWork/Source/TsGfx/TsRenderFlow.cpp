#include "TsGfx.h"


TsBool TsRenderFlow::Render( TsDeviceContext* pDC ,TsDrawQueue* pQue )
{
	
	if (pDC == nullptr || pQue == nullptr)
		return TS_FALSE;

	for( auto it : m_renderPass )
	{
		it->Begin(pDC);
		pQue->Render(pDC);
		it->End(pDC);
	}
	return TS_TRUE;
}

TsRenderPass* TsRenderFlow::GetRenderPass(TsString name)
{
	TS_HASH hash = TSUT::StringToHash(name);

	for (auto it : m_renderPass)
	{
		if (it->GetHashCode() == hash)
			return it;
	}

	return nullptr;
}
TsRenderPass* TsRenderFlow::GetRenderPass(TsInt index)
{
	if ((unsigned)index > m_renderPass.size())
		return nullptr;
	else
		return m_renderPass[index];
}

TsBool TsRenderFlow::SetRenderPass(TsRenderPass* pass, TsInt index)
{
	if ((unsigned)index >= m_renderPass.size())
		m_renderPass.push_back(pass);
	else
		m_renderPass[index] = pass;

	return TS_TRUE;
}

TsInt		  TsRenderFlow::GetFlowSize()
{
	return m_renderPass.size();
}