#pragma once

class TsSamplerState : public TsNameObject
{
public:
    TsSamplerState( const TsString& name = "Sampler" );
    ID3D11SamplerState* GetSamplerState()const;
    TsBool SetD3DSamplerState( ID3D11SamplerState* pD3DSampler );
private:
    ID3D11SamplerState* m_pD3D11Sampler;
};