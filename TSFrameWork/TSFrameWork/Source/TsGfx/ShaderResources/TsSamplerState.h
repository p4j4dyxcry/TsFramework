//**********************************************************
//! TsSamplerState.h
//! Sampler State.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

class TsSamplerState : public TsNameObject
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsSamplerState( const TsString& name = "" );
    ID3D11SamplerState* GetSamplerState()const;
    TsBool SetD3DSamplerState( ID3D11SamplerState* pD3DSampler );
private:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    ID3D11SamplerState* m_pD3D11Sampler;
};