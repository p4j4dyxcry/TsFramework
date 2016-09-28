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

TsBool	TsRenderFlow::LoadFlowFromXML( TsDevice* pDev, const TsString& file )
{
	TsXML xml;

	xml.LoadXML( TSUT::Resource::GetShaderFlowDirectory() + file );

	auto pass = xml.GetRootNode()->GetFirstChild();

	while( pass != nullptr )
	{
		if( pass->GetName() == "Pass" )
		{
				TsString passName = pass->GetAttribute( "Name" )->GetStringValue();
				TsRenderPass* renderPass = TsRenderPass::CreateObject( passName);
			
				TsString shaderName = pass->GetAttribute( "Shader" )->GetStringValue();
				TsShaderEffect* pShaderEffect = TsShaderEffect::Find( shaderName );
				renderPass->SetShader( pShaderEffect );
				TsXMLElement* inputSlot = pass->FindChild("Input")->GetFirstChild();
				for( TsInt index = 0; inputSlot != nullptr;  inputSlot = inputSlot->Next() )
				{
					auto slot = inputSlot->GetAttribute( "Slot" );
					if( slot == nullptr )
						continue;
					TsString rtName = slot->GetStringValue();
					TsRenderTarget* rtv = TsRenderTarget::Find( rtName );
					renderPass->SetInputSlot( index, rtv);
					++index;
				}

				TsXMLElement * outputSlot = pass->FindChild( "Output" )->GetFirstChild();

				for( TsInt index = 0; outputSlot != nullptr; outputSlot = outputSlot->Next() )
				{
					auto slot = outputSlot->GetAttribute( "Slot" );

					if( slot == nullptr )
						continue;
					TsString rtName = slot->GetStringValue();
					if( outputSlot->GetName() == "Slot" )
					{
						TsRenderTarget* rtv = TsRenderTarget::Find( rtName );
						renderPass->SetOutputSlot( index , rtv );
						++index;
					}
					else if( outputSlot->GetName() == "Depth" )
					{
						TsDepthStencil* rtv = TsDepthStencil::Find( rtName );
						renderPass->SetDepthSlot( rtv );
					}

				}						
			m_renderPass.push_back( renderPass );
		}
		pass = pass->Next();
	}
	return TS_TRUE;
} 