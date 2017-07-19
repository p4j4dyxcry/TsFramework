#include "../TsGfx.h"


TsBool TsRenderFlow::Render( TsDeviceContext* pDC ,TsDrawQueue* pQue )
{
    
    if (pDC == nullptr || pQue == nullptr)
        return TS_FALSE;

    for( auto it : m_renderPass )
    {
        it->Begin(pDC);
        it->Render( pQue,pDC );
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
    if (static_cast<unsigned>(index) > m_renderPass.size())
        return nullptr;
    else
        return m_renderPass[index];
}

TsBool TsRenderFlow::SetRenderPass(TsRenderPass* pass, TsInt index)
{
    if (static_cast<unsigned>(index) >= m_renderPass.size())
        m_renderPass.push_back(pass);
    else
        m_renderPass[index] = pass;

    return TS_TRUE;
}

TsInt TsRenderFlow::GetFlowSize() const
{
    return m_renderPass.size();
}

TsBool TsRenderFlow::LoadFlowFromXML( TsDevice* pDev, const TsString& file )
{
    TsXML xml;

    xml.LoadXML( TSUT::Resource::GetShaderFlowDirectory() + file );
    auto pElementPass = xml.GetRootNode()->GetFirstChild();

    while( pElementPass != nullptr )
    {
        if( pElementPass->GetName() == "Pass" )
        {
                TsString passName = pElementPass->GetAttribute( "Name" )->GetStringValue();

                TsRenderPass* renderPass;
                if( passName == "Clear" )
                {
                    renderPass = TsNew( TsClearPass );
                }
                else
                {
                    renderPass = TsNew( TsRenderPass );
                }

                renderPass->SetName( passName );
                renderPass->LoadShaderFromXMLElement( pDev , pElementPass );
                renderPass->LoadIOSlotFromXMLElement( pDev , pElementPass );

            m_renderPass.push_back( renderPass );
        }
        pElementPass = pElementPass->Next();
    }
    return TS_TRUE;
} 