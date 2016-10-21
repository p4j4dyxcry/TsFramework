#include "TsGfx.h"

TS_INSTANTIATE_NAME_OBJ_LIST( TsRenderPass );

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
TsBool TsRenderPass::ApplyRTV( TsDeviceContext* pDC )
{
    //! set render target
    for( int i = 0; i < TsDeviceContext::MAX_RTs; ++i )
        pDC->SetRT( i , m_pOutputSlot[i] );

    //! set depth 
    pDC->SetDepthStencil( m_pDepthStencil );

    // apply
    pDC->ApplyRenderTargets();

    return TS_TRUE;
}


TsBool TsRenderPass::Begin( TsDeviceContext* pDc )
{

    ApplyRTV( pDc );

    //! set shader
    pDc->SetShader( m_pShader );

    //! set inputlayout
    pDc->ApplyInputLayout();

    return TS_TRUE;
}
TsBool TsRenderPass::End( TsDeviceContext* pDC )
{
    //! set render target
    for( int i = 0; i < TsDeviceContext::MAX_RTs; ++i )
        pDC->SetRT( i , nullptr );

    //! set depth 
    pDC->SetDepthStencil( nullptr );

    return TS_TRUE;
}

TsBool TsRenderPass::Render( TsDrawQueue* pQue , TsDeviceContext* pDC )
{
    pQue->Render( pDC );
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

TsBool TsRenderPass::SetDepthSlot( TsDepthStencil* depth )
{
    m_pDepthStencil = depth;
    return TS_TRUE;
}

TsBool TsRenderPass::LoadShaderFromXMLElement( TsDevice* pDev , TsXMLElement * pElement )
{
    TsString shaderName = pElement->GetAttribute( "Shader" )->GetStringValue();
    TsShaderEffect* pShaderEffect = TsShaderEffect::Find( shaderName );
    if( pShaderEffect == nullptr )
    {
        pShaderEffect = TsNew( TsShaderEffect );
        pShaderEffect->LoadPackageFromXml( pDev , shaderName + ".ts_shaderPackage" );
    }
    SetShader( pShaderEffect );

    return TS_TRUE;
}
TsBool TsRenderPass::LoadIOSlotFromXMLElement( TsDevice* pDev , TsXMLElement * pElement )
{
    TsInt rtvIndex = 0;

    TsXMLElement* inputSlot = pElement->FindChild( "Input" )->GetFirstChild();
    for( ; inputSlot != nullptr; inputSlot = inputSlot->Next() )
    {
        auto slot = inputSlot->GetAttribute( "Slot" );
        if( slot == nullptr )
            continue;
        TsString rtName = slot->GetStringValue();
        TsRenderTarget* rtv = TsRenderTarget::Find( rtName );
        SetInputSlot( rtvIndex , rtv );
        ++rtvIndex;
    }

    TsXMLElement * outputSlot = pElement->FindChild( "Output" )->GetFirstChild();

    rtvIndex = 0;
    for( ; outputSlot != nullptr; outputSlot = outputSlot->Next() )
    {
        auto slot = outputSlot->GetAttribute( "Slot" );

        if( slot == nullptr )
            continue;
        TsString rtName = slot->GetStringValue();
        if( outputSlot->GetName() == "Slot" )
        {
            TsRenderTarget* rtv = nullptr;
            if( rtName == "Main" )
                rtv = pDev->GetDC()->GetMainRTV();
            else
                rtv = TsRenderTarget::Find( rtName );

            SetOutputSlot( rtvIndex , rtv );
            ++rtvIndex;
        }
        else if( outputSlot->GetName() == "Depth" )
        {
            TsDepthStencil* rtv = nullptr;
            if( rtName == "Main" )
                rtv = pDev->GetDC()->GetMainDSV();
            else
                rtv = TsDepthStencil::Find( rtName );
            SetDepthSlot( rtv );
        }
    }
    return TS_TRUE;
}
