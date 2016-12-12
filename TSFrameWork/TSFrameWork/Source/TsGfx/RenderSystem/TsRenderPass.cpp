#include "../../../TsAfx.h"

TS_INSTANTIATE_NAME_OBJ_LIST( TsRenderPass );

//----------------------------------------------------------
//! Constructor
//----------------------------------------------------------
TsRenderPass::TsRenderPass():
m_pShader(nullptr),
m_pDepthStencilState(nullptr),
m_pDepthStencilView(nullptr),
m_pRasterizerState(nullptr)
{
    for( int i = 0; i < TsDeviceContext::MAX_RTs; ++i )
    {
        m_pInputSlot[i] = nullptr;
        m_pOutputSlot[i] = nullptr;
    }
}
//----------------------------------------------------------
//! Destructor
//----------------------------------------------------------
TsRenderPass::~TsRenderPass()
{

}

//----------------------------------------------------------
//! RenderTargetをパイプライン上に確定させる
//----------------------------------------------------------
TsBool TsRenderPass::ApplyRTV( TsDeviceContext* pDC )
{
    //! set render target
    for( int i = 0; i < TsDeviceContext::MAX_RTs; ++i )
        pDC->SetRT( i , m_pOutputSlot[i] );

    //! set depth 
    pDC->SetDepthStencilView( m_pDepthStencilView );

    if (m_pOutputSlot[0])
    {
        TsViewport viewport;
        TsInt2 sz = m_pOutputSlot[0]->GetRTVSize();
        viewport.Create(sz.x, sz.y);
        pDC->SetViewport(&viewport);
    }

    // apply
    pDC->ApplyRenderTargets();

    return TS_TRUE;
}

//----------------------------------------------------------
//! 事前処理
//----------------------------------------------------------
TsBool TsRenderPass::Begin( TsDeviceContext* pDc )
{
    ApplyRTV( pDc );

    //! set shader
    pDc->SetShader( m_pShader );

    //! set inputlayout
    pDc->ApplyInputLayout();

    //! SetInputSlot
    for( TsInt i = 0; m_pInputSlot[i] != nullptr; ++i )
    {
        pDc->SetTexture( i , m_pInputSlot[i] , TS_SHADER_TYPE::PIXEL_SHADER );
    }

    //! Set Depth Stencil
    if( m_pDepthStencilState )
    {
        pDc->SetDepthStencilState( m_pDepthStencilState );
        pDc->ApplyDepthStencil();
    }

    //! Set Rasterize
    if( m_pRasterizerState )
    {
        pDc->SetRasterizerState( m_pRasterizerState );
        pDc->ApplyRasterizer();
    }

    //! Set BlendState
    if( m_pBlendState )
    {
        pDc->SetBlendState( m_pBlendState );
        pDc->ApplyBlendState();
    }

    return TS_TRUE;
}

//----------------------------------------------------------
//! 終了処理
//----------------------------------------------------------
TsBool TsRenderPass::End( TsDeviceContext* pDC )
{
    //パイプライン上の全てのRenderTargetをクリアする
    for( int i = 0; i < TsDeviceContext::MAX_RTs; ++i )
        pDC->SetRT( i , nullptr );

    //パイプライン上の全ての深度/ステンシルをクリアする
    pDC->SetDepthStencilView( nullptr );

    return TS_TRUE;
}

//----------------------------------------------------------
//! QueueのRenderingを実行
//----------------------------------------------------------
TsBool TsRenderPass::Render( TsDrawQueue* pQue , TsDeviceContext* pDC )
{
    if( pDC == nullptr )
        return TS_FALSE;
    if( pQue == nullptr )
        return TS_FALSE;

    pQue->Render( pDC );
    return TS_TRUE;
}

//----------------------------------------------------------
//! InputSlotを設定する
//----------------------------------------------------------
TsBool TsRenderPass::SetInputSlot( TsInt index , TsTexture *rtv)
{
    //領域として t0 ~ t7までを割り当てる
    if( abs( index ) >= TsDeviceContext::MAX_RTs )
        return TS_FALSE;
    m_pInputSlot[index] = rtv;

    return TS_TRUE;
}

//----------------------------------------------------------
//! 出力スロットを設定する
//----------------------------------------------------------
TsBool TsRenderPass::SetOutputSlot(TsInt index, TsRenderTarget *rtv )
{
    if( abs( index ) >= TsDeviceContext::MAX_RTs )
        return TS_FALSE;
    m_pOutputSlot[index] = rtv;

    return TS_TRUE;
}

//----------------------------------------------------------
//! 入力スロットにバインドされているテクスチャを取得する
//----------------------------------------------------------
TsTexture* TsRenderPass::GetInputSlot( TsInt index )
{
    if( index > TsDeviceContext::MAX_RTs || index < 0)
        return nullptr;
    return m_pInputSlot[ index ];
}

//----------------------------------------------------------
//! 出力スロットにバインドされているRenderTargetを取得する
//----------------------------------------------------------
TsRenderTarget* TsRenderPass::GetOutputSlot( TsInt index /* = 0 */)
{
    if( index > TsDeviceContext::MAX_RTs || index < 0 )
        return nullptr;
    return m_pOutputSlot[index];
}

//----------------------------------------------------------
//! Shader Effect を設定する
//----------------------------------------------------------
TsBool TsRenderPass::SetShader( TsShaderEffect* se )
{
    m_pShader = se;
    return TS_TRUE;
}

//----------------------------------------------------------
//! Depth/Stencilの出力を設定する
//----------------------------------------------------------
TsBool TsRenderPass::SetDepthSlot( TsDepthStencilView* depth )
{
    m_pDepthStencilView = depth;
    return TS_TRUE;
}

//----------------------------------------------------------
//! XMLからシェーダエフェクトを読み込む
//----------------------------------------------------------
TsBool TsRenderPass::LoadShaderFromXMLElement( TsDevice* pDev , TsXMLElement * pElement )
{
    TsString shaderName = pElement->GetAttribute( "Shader" )->GetStringValue();
    TsShaderEffect* pShaderEffect = TsShaderEffect::Find( shaderName );
    if( pShaderEffect == nullptr )
    {
        pShaderEffect = TsNew( TsShaderEffect );
        pShaderEffect->LoadPackageFromXml( pDev , shaderName + ".ts_shaderPackage" );
    }
    m_pDepthStencilState = pShaderEffect->GetDepthStencilState();
    m_pRasterizerState = pShaderEffect->GetRasterizeState();
    m_pBlendState = pShaderEffect->GetBlendState();
    SetShader( pShaderEffect );

    return TS_TRUE;
}

//----------------------------------------------------------
//! XMLからRenderPass情報を読み込む
//----------------------------------------------------------
TsBool TsRenderPass::LoadIOSlotFromXMLElement( TsDevice* pDev , TsXMLElement * pElement )
{
    TsInt rtvIndex = 0;

    TsXMLElement* inputSlot = pElement->FindChild( "Input" )->GetFirstChild();
    for( ; inputSlot != nullptr; inputSlot = inputSlot->Next() )
    {
        //----------------------------------------------------------
        //! インプットスロットの読み込み
        //----------------------------------------------------------
        auto slot = inputSlot->GetAttribute( "Slot" );
        if( slot == nullptr )
            continue;
        if( inputSlot->GetName() == "Slot" )
        {
            TsString rtName = slot->GetStringValue();
            TsRenderTarget* rtv = nullptr;
            if( rtName == "Main" )
                rtv = pDev->GetDC()->GetMainRTV();
            else
                rtv = TsRenderTarget::Find( rtName );
            if( rtv != nullptr )
            {
                SetInputSlot( rtvIndex , rtv );
            }
            else
            {
                TsTexture* pTex = TsResourceManager::Find<TsTexture>( rtName );
                if( pTex )
                    SetInputSlot( rtvIndex , pTex );
            }
            ++rtvIndex;
        }
        else if( inputSlot->GetName() == "Depth" )
        {
            TsString rtName = slot->GetStringValue();
            TsDepthStencilView* dsv = nullptr;
            if( rtName == "Main" )
                dsv = pDev->GetDC()->GetMainDSV();
            else
                dsv = TsDepthStencilView::Find( rtName );
            SetInputSlot( rtvIndex , dsv );
            ++rtvIndex;
        }
    }

    //----------------------------------------------------------
    //! OutPutスロットの読み込み
    //----------------------------------------------------------
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
            TsDepthStencilView* rtv = nullptr;
            if( rtName == "Main" )
                rtv = pDev->GetDC()->GetMainDSV();
            else
                rtv = TsDepthStencilView::Find( rtName );
            SetDepthSlot( rtv );
        }
    }
    return TS_TRUE;
}
