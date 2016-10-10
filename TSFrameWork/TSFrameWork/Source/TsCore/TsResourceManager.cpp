#include "../../TsAfx.h"

TsDevice* TsResourceManager::m_pDevice = nullptr;
TsMap<TS_HASH , TsSamplerState*> TsResourceManager::m_SamplerLibrary;

TsBool TsResourceManager::Initialize( TsDevice* pDev )
{
    m_pDevice = pDev;
    InitializeSampler( pDev );

    return TS_TRUE;
}

TsBool TsResourceManager::InitializeSampler( TsDevice * pDev )
{
    D3D11_SAMPLER_DESC desc;
    desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;

    desc.BorderColor[0] = 
    desc.BorderColor[1] =
    desc.BorderColor[2] =
    desc.BorderColor[3] = 0.0f;

    desc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;

    desc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
    desc.MaxAnisotropy = 16;
    desc.MinLOD = 0;
    desc.MaxLOD = D3D11_FLOAT32_MAX;

    desc.MipLODBias = 0;
    std::pair<TS_HASH , TsSamplerState*> pair( TSUT::StringToHash("Default") , pDev->CreateSamplerState( desc ) );
    m_SamplerLibrary.insert(pair);

    return TS_TRUE;
}

TsSamplerState* TsResourceManager::GetSamplerState( const TsString& name )
{
    TS_HASH hash = TSUT::StringToHash( name );
    return m_SamplerLibrary.find( hash )->second;
}