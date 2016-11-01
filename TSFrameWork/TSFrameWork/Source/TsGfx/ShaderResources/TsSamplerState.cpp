#include "../TsGfx.h"

TsSamplerState::TsSamplerState( const TsString& name )
{
    SetName( name );
}
ID3D11SamplerState* TsSamplerState::GetSamplerState()const
{
    return m_pD3D11Sampler;
}
TsBool TsSamplerState::SetD3DSamplerState( ID3D11SamplerState* pD3DSampler )
{
    m_pD3D11Sampler = pD3DSampler;
    return TS_TRUE;
}