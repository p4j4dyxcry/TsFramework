
#include "../TsGfx.h"


TsBool TsTexture2D::CreateTextureFromBinary(TsDevice* pDevice,
                                            void* pBinary,
                                            size_t sz,
                                            DXGI_FORMAT format,
                                            TsUint width,
                                            TsUint height)
{
    ID3D11Device* pDev = pDevice->GetDevD3D();

    // Step 1 Create Texture
    {
        TS_TEX2D_DESC texDesc(width, height, format);
        texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
        texDesc.Usage = D3D11_USAGE_STAGING;
        HRESULT hr = pDev->CreateTexture2D(&texDesc, nullptr, &m_tex2d);

        if (FAILED(hr))
        {
            return TS_FALSE;
        }

    }

    // Step 3 Create Shader Resource View
    {
        HRESULT hr = pDev->CreateShaderResourceView(m_tex2d, nullptr, &m_srv);

        if (FAILED(hr))
        {
            return TS_FALSE;
        }
    }

    // Step 4 Create Sample State 
    // *RenderTarget ごとにサンプラを持つ必要はない。

    return TS_TRUE;
}