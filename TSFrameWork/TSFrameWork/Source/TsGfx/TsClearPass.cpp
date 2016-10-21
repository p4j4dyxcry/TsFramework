#include "TsGfx.h"

TsClearPass::TsClearPass()
    :TsRenderPass(),
    m_clearColor( TsFloat4( 0 , 1 , .5f , .5f ) )
{
    
}
TsClearPass::~TsClearPass(){}

TsBool TsClearPass::Begin( TsDeviceContext* pDC)
{
    ApplyRTV( pDC );

    return TS_TRUE;
}
TsBool TsClearPass::Render( TsDrawQueue* pQue , TsDeviceContext* pDC )
{
    ( void )pQue;
    pDC->Clear( m_clearColor );

    return TS_TRUE;
}

TsBool TsClearPass::LoadShaderFromXMLElement( TsDevice* pDev , TsXMLElement * pElement )
{
    ( void )pDev;
    ( void )pElement;
    return TS_TRUE;
}