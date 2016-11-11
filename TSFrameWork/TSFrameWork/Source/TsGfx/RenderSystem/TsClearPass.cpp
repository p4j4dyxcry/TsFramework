#include "../TsGfx.h"

//----------------------------------------------------------
//! Constructor
//----------------------------------------------------------
TsClearPass::TsClearPass()
    :TsRenderPass(),
    m_clearColor(  )
{
    
}

//----------------------------------------------------------
//! Destructor
//----------------------------------------------------------
TsClearPass::~TsClearPass(){}

//----------------------------------------------------------
//! Begin
//----------------------------------------------------------
TsBool TsClearPass::Begin( TsDeviceContext* pDC)
{
    // Render Target の Set & Apply　のみ行う
    ApplyRTV( pDC );

    return TS_TRUE;
}

//----------------------------------------------------------
//! Render
//----------------------------------------------------------
TsBool TsClearPass::Render( TsDrawQueue* pQue , TsDeviceContext* pDC )
{
    // Render Pass でサーフェイスをクリアする
    ( void )pQue;
    pDC->Clear( m_clearColor );

    return TS_TRUE;
}
//----------------------------------------------------------
//! Load Shader From XML Element
//----------------------------------------------------------
TsBool TsClearPass::LoadShaderFromXMLElement( TsDevice* pDev , TsXMLElement * pElement )
{
    //! Clear はシェーダのロードは行わない。
    ( void )pDev;
    ( void )pElement;
    return TS_TRUE;
}