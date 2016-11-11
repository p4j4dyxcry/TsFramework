#include "../TsGfx.h"

TsDepthStencilState::TsDepthStencilState()
    :m_depthtTestFunc( TS_COMPARISON_FUNC::LESS ) ,
    m_zEnable( TS_TRUE ) ,
    m_zRwriteEnable( TS_TRUE ),
    m_pDepteStencil( nullptr )
{

}

TsDepthStencilState::~TsDepthStencilState()
{
    TsSafeDelete(m_pDepteStencil);
}

TsBool TsDepthStencilState::CreateDepthStencil( TsDevice* pDev )
{
    TS_DEPTH_STENCIL_DESC desc;
    desc.DepthWriteMask = ( D3D11_DEPTH_WRITE_MASK )m_zRwriteEnable;
    desc.DepthEnable = m_zRwriteEnable;
    desc.DepthFunc = ( D3D11_COMPARISON_FUNC )m_depthtTestFunc;

    HRESULT hr = pDev->GetDevD3D()->CreateDepthStencilState( &desc , &m_pDepteStencil );
    if( !SUCCEEDED( hr ) )
        return TS_FALSE;
    return TS_TRUE;
}

TsBool TsDepthStencilState::SetZEnable( TsBool flag )
{
    m_zEnable = flag;
    return TS_TRUE;
}
TsBool TsDepthStencilState::SetZWriteEnable( TsBool flag )
{
    m_zRwriteEnable = flag;
    return TS_TRUE;
}
TsBool TsDepthStencilState::SetDepthTestFunc( TS_COMPARISON_FUNC func )
{
    m_depthtTestFunc = func;
    return TS_TRUE;
}

TsBool TsDepthStencilState::SetDepthTestFuncByString( const TsString& str )
{
    if( str == "DNEVER" )
        m_depthtTestFunc = TS_COMPARISON_FUNC::NEVER;
    else if( str == "DLESS" )
        m_depthtTestFunc = TS_COMPARISON_FUNC::LESS;
    else if( str == "DEQUAL" )
        m_depthtTestFunc = TS_COMPARISON_FUNC::EQUAL;
    else if( str == "LESS_EQUAL" )
        m_depthtTestFunc = TS_COMPARISON_FUNC::LESS_EQUAL;
    else if( str == "GREATER" )
        m_depthtTestFunc = TS_COMPARISON_FUNC::GREATER;
    else if( str == "NOT_EQUAL" )
        m_depthtTestFunc = TS_COMPARISON_FUNC::NOT_EQUAL;
    else if( str == "GREATER_EQUAL" )
        m_depthtTestFunc = TS_COMPARISON_FUNC::GREATER_EQUAL;
    else if( str == "ALWAYS" )
        m_depthtTestFunc = TS_COMPARISON_FUNC::ALWAYS;

    return TS_TRUE;
}
