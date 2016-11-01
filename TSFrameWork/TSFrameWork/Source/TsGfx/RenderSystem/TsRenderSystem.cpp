#include "../../../TsAfx.h"
TsRenderSystem::TsRenderSystem()
{

}
TsRenderSystem::~TsRenderSystem()
{

}
TsBool TsRenderSystem::CallRender(TsDeviceContext* pDC)
{
    m_PreFlowAndQue.Render(pDC);
    m_FlowAndQue.Render(pDC);
    m_PostFlowAndQue.Render(pDC);

    return TS_TRUE;
}

TsBool TsRenderSystem::SetDrawQue(TsDrawQueue* pDrawQue, TARGET_FLOW targetFlow )
{
    switch (targetFlow)
    {
    case TARGET_FLOW::PRE_RENDERER      :m_PreFlowAndQue.pQue = pDrawQue; break;
    case TARGET_FLOW::DEFAULT_RENDERER  :m_FlowAndQue.pQue = pDrawQue; break;
    case TARGET_FLOW::POST_RENDERER     :m_PostFlowAndQue.pQue = pDrawQue; break;
    default:return TS_FALSE;
    }
    return TS_TRUE;
}
TsBool TsRenderSystem::SetShaderFlow(TsRenderFlow* pFlow, TARGET_FLOW targetFlow )
{
    switch (targetFlow)
    {
    case TARGET_FLOW::PRE_RENDERER:     m_PreFlowAndQue.pFlow = pFlow; break;
    case TARGET_FLOW::DEFAULT_RENDERER: m_FlowAndQue.pFlow = pFlow; break;
    case TARGET_FLOW::POST_RENDERER:    m_PostFlowAndQue.pFlow = pFlow; break;
    default:return TS_FALSE;
    }
    return TS_TRUE;
}

TsTexture* TsRenderSystem::FindTextureResource(const TsString& name)
{
    TS_HASH hash = TSUT::StringToHash(name);
    for each(auto it in m_shaderResourceList)
    {
        if (hash == it->GetHashCode())
            return it;
    }
    return nullptr;
}

TsBool TsRenderSystem::LoadRenderSystemFromXML( TsDevice*pDev , const TsString& name )
{
    TsXML xml;
    xml.LoadXML( TSUT::Resource::GetRenderSystemDirectory() + name );

    auto elm = xml.GetRootNode()->GetFirstChild();
    while( elm != nullptr )
    {
        if( elm->GetName() == "Resource" )
        {
            TsString path = elm->GetAttribute( "resource" )->GetStringValue();
            TSUT::TsFilePathAnalyzer ana( path );
            if( ana.GetExtension() == ".ts_shaderReource" )
            {
                LoadShaderResourceFromXML( pDev , path );
            }
        }
        else if( elm->GetName() == "PreRender" )
        {
            TsString path = elm->GetAttribute( "flow" )->GetStringValue();
            TSUT::TsFilePathAnalyzer ana( path );
            if( ana.GetExtension() == ".ts_shaderflow" )
            {
                TsRenderFlow* flow = TsNew( TsRenderFlow );
                flow->LoadFlowFromXML( pDev , path );
                m_PreFlowAndQue.pFlow = flow;
            }
        }
        else if( elm->GetName() == "Render" )
        {
            TsString path = elm->GetAttribute( "flow" )->GetStringValue();
            TSUT::TsFilePathAnalyzer ana( path );
            if( ana.GetExtension() == ".ts_shaderflow" )
            {
                TsRenderFlow* flow = TsNew( TsRenderFlow );
                flow->LoadFlowFromXML( pDev , path );
                m_FlowAndQue.pFlow = flow;
            }
        }
        else if( elm->GetName() == "PostRender" )
        {
            TsString path = elm->GetAttribute( "flow" )->GetStringValue();
            TSUT::TsFilePathAnalyzer ana( path );
            if( ana.GetExtension() == ".ts_shaderflow" )
            {
                TsRenderFlow* flow = TsNew( TsRenderFlow );
                flow->LoadFlowFromXML( pDev , path );
                m_PostFlowAndQue.pFlow = flow;
            }
        }
        elm = elm->Next();
    }
    return TS_TRUE;
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
            TsTexture2D * pTex = TsNew( TsTexture2D );
            TsString name = elm->GetAttribute("Name")->GetStringValue();
            TsString path = elm->GetAttribute("Path")->GetStringValue();
            auto srv = TsDirectXTex::LoadFromFile(pDev->GetDevD3D(), path.c_str());
            pTex->SetName(name);
            pTex->SetSRV(srv);
            
            m_shaderResourceList.push_back(pTex);
            TsResourceManager::RegisterResource<TsTexture2D>( pTex , name );
        }
        if (elm->GetName() == "RenderTarget")
        {
            TsString name = elm->GetAttribute("Name")->GetStringValue();
            TsString format = elm->GetAttribute("Format")->GetStringValue();
            TsInt2   size = elm->GetAttribute("Size")->GetInt2Value();

            if( size.x < 0 )
                size = pDev->GetDC()->GetMainRTV()->GetRTVSize();
            auto rtv = TsRenderTarget::CreateRTV(name, *pDev, size.x, size.y,TSShaderUT::ComvertTextureFormat(format));

            m_shaderResourceList.push_back(rtv);
        }

        if( elm->GetName() == "Depth" )
        {
            TsString name = elm->GetAttribute( "Name" )->GetStringValue();
            TsString format = elm->GetAttribute( "Format" )->GetStringValue();
            TsInt2   size = elm->GetAttribute( "Size" )->GetInt2Value();

            if( size.x < 0 )
                size = pDev->GetDC()->GetMainRTV()->GetRTVSize();
            auto dsv = TsDepthStencilView::CreateDSV( name , *pDev , size.x , size.y , TSShaderUT::ComvertTextureFormat( format ) );

            m_shaderResourceList.push_back( dsv );
        }
        elm = elm->Next();
        
    }
    return TS_TRUE;
}