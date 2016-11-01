#include "../TsGfx.h"

TsRasterizerState::TsRasterizerState()
    :m_cullMode( TS_CULL_MODE::BACK ),
    m_fillMode( TS_FILL_MODE::SOLID ),
    m_antiAlias( TS_TRUE ) ,
    m_pD3DRasterizerState(nullptr)
{ 
}

TsBool TsRasterizerState::SetCullMode( TS_CULL_MODE mode )
{
    m_cullMode = mode;
    return TS_TRUE;
}

TsBool TsRasterizerState::SetFillMode( TS_FILL_MODE mode )
{
    m_fillMode = mode;
    return TS_TRUE;
}

TsBool TsRasterizerState::SetCullModeByString( const TsString& mode )
{
    if( mode == "CCW" )
        m_cullMode = TS_CULL_MODE::BACK;
    if( mode == "CW" )
        m_cullMode = TS_CULL_MODE::FRONT;
    if( mode == "NONE" )
        m_cullMode = TS_CULL_MODE::NONE;

    return TS_TRUE;
}

TsBool TsRasterizerState::SetFillModeByString( const TsString& mode )
{
    if( mode == "SOLID" )
        m_fillMode = TS_FILL_MODE::SOLID;
    if( mode == "WIREFRAME" )
        m_fillMode = TS_FILL_MODE::WIREFRAME;

    return TS_TRUE;
}

TsBool TsRasterizerState::SetAntiAliasMode( TsBool flag )
{
    m_antiAlias = flag;
    return TS_TRUE;
}

TsBool TsRasterizerState::CreateRasterizerState( TsDevice* pDev )
{
    TsSafeRelease( m_pD3DRasterizerState );

    HRESULT hr;
    D3D11_RASTERIZER_DESC rasterDesc;

    // setup the raster description which will determine how and what polygons will be drawn
    rasterDesc.AntialiasedLineEnable    = m_antiAlias;
    rasterDesc.CullMode                 = (D3D11_CULL_MODE)m_cullMode;
    rasterDesc.FillMode                 = ( D3D11_FILL_MODE )m_fillMode;
    rasterDesc.DepthBias                = 0;
    rasterDesc.DepthBiasClamp           = 0.0f;
    rasterDesc.SlopeScaledDepthBias     = 0.0f;
    rasterDesc.DepthClipEnable          = TS_TRUE;
    rasterDesc.FrontCounterClockwise    = TS_FALSE;
    rasterDesc.MultisampleEnable        = TS_FALSE;
    rasterDesc.ScissorEnable            = TS_FALSE;

    // create the rasterrizer state from the description we just filled out 
    hr = pDev->GetDevD3D()->CreateRasterizerState( &rasterDesc , &m_pD3DRasterizerState );
    if( FAILED( hr ) )
    {
        return TS_FALSE;
    }
    
    return TS_TRUE;
}

TsBool TsRasterizerState::operator == ( const TsRasterizerState& rasterizer )
{
    TsBool b = m_cullMode == rasterizer.m_cullMode;
    b &= m_fillMode == rasterizer.m_fillMode;
    b &= m_antiAlias == rasterizer.m_antiAlias;
    return b;
}

TsBool TsRasterizerState::operator != ( const TsRasterizerState& rasterizer )
{
    return !( ( *this ) == rasterizer );
}