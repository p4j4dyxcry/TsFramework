//!*******************************************************
//! TsResourceDesc.h
//!
//! lapping dx11 descs. ( add Constructor )
//!
//! © 2016 Yuki Tsuneyama

#pragma once

//----------------------------------------------------------
// Define of dx11 multi sample desc.
//----------------------------------------------------------
struct TS_SAMPLE_DESC : DXGI_SAMPLE_DESC
{
    TS_SAMPLE_DESC( TsInt count = 1, TsInt quality = 0 )
    {
        Count   = (TsUint)count;
        Quality = (TsUint)quality;
    }
};

//----------------------------------------------------------
// Define of dx11 DXGI_MODE_DESC
//----------------------------------------------------------
struct TS_MODE_DESC : DXGI_MODE_DESC
{
    TS_MODE_DESC( TsUint width , 
                  TsUint height , 
                  TsInt2 refreshRato , 
                  DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM ,
                  DXGI_MODE_SCANLINE_ORDER scanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED ,
                  DXGI_MODE_SCALING  scaling = DXGI_MODE_SCALING_UNSPECIFIED )
    {
        Width = width;                          // バックバッファの幅
        Height = height;                        // バックバッファの高さ
        Format = format;                        // バックバッファフォーマット(R,G,B 範囲０．０から１．０)
        RefreshRate.Numerator = refreshRato.x;  // リフレッシュレート（分母）
        RefreshRate.Denominator = refreshRato.y;// リフレッシュレート（分子）
        ScanlineOrdering = scanlineOrdering;
        Scaling = scaling;
    }
};

//----------------------------------------------------------
// Define of dx11 DXGI_SWAP_CHAIN_DESC
//----------------------------------------------------------
struct TS_SWAP_CHAIN_DESC : DXGI_SWAP_CHAIN_DESC
{
    //todo
    TS_SWAP_CHAIN_DESC( HWND hWnd,
                        TsUint width , 
                        TsUint height , 
                        TsInt2 refreshRato ,
                        TsBool isFullScreen = TS_FALSE)
    {		
        BufferCount = 1;                                            // バックバッファの数
        BufferDesc = TS_MODE_DESC( width , height , refreshRato );
        BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;              // バックバッファの使用方法
        OutputWindow = hWnd;                                        // 関連付けるウインドウ
        SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        SampleDesc = TS_SAMPLE_DESC();
        Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        Windowed = !isFullScreen;
    }
};
// dx11 sampler desc.
struct TS_SAMPLER_DESC : D3D11_SAMPLER_DESC
{
    //todo
};

// dx11 buffer desc.
struct TS_BUFFER_DESC : D3D11_BUFFER_DESC
{
    //todo
};

//----------------------------------------------------------
// Define of dx11 D3D11_DEPTH_STENCIL_DESC
//----------------------------------------------------------
struct TS_DEPTH_STENCIL_DESC : D3D11_DEPTH_STENCIL_DESC
{
    TS_DEPTH_STENCIL_DESC()
    {
        ZeroMemory( this , sizeof( TS_DEPTH_STENCIL_DESC ) );
        DepthEnable = TS_TRUE;
        DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        DepthFunc = D3D11_COMPARISON_LESS;

        // set up the description of the stencl state
        StencilEnable = TS_TRUE;
        StencilReadMask = 0xFF;
        StencilWriteMask = 0xFF;

        // stencil operation if pixel is front-facing
        FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        // stencil operation if pixel is　back-facing
        BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    }
};

//----------------------------------------------------------
// Define of dx11 D3D11_SHADER_RESOURCE_VIEW_DESC
//----------------------------------------------------------
struct TS_SRV_DESC : D3D11_SHADER_RESOURCE_VIEW_DESC
{
    TS_SRV_DESC( DXGI_FORMAT format )
    {
        ZeroMemory( this , sizeof( TS_SRV_DESC ) );
        Format = format;
        ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    }
};

//----------------------------------------------------------
// Define of dx11 D3D11_RENDER_TARGET_VIEW_DESC
//----------------------------------------------------------
struct TS_RTV_DESC : D3D11_RENDER_TARGET_VIEW_DESC
{
    TS_RTV_DESC( DXGI_FORMAT format )
    {
        Format = format;
        ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

        Buffer.FirstElement = 0;
        Buffer.ElementOffset = 0;

        Texture1D.MipSlice = 0;
        Texture2D.MipSlice = 0;
        Texture3D.MipSlice = 0;

        Texture2DMS.UnusedField_NothingToDefine = 0;

        Texture1DArray.ArraySize = 0;
        Texture1DArray.FirstArraySlice = 0;
        Texture1DArray.MipSlice = 0;

        Texture2DArray.ArraySize = 0;
        Texture2DArray.FirstArraySlice = 0;
        Texture2DArray.MipSlice = 0;

        Texture2DMSArray.ArraySize = 0;
        Texture2DMSArray.FirstArraySlice = 0;
    }
};

//----------------------------------------------------------
// Define of dx11 D3D11_TEXTURE2D_DESC
//----------------------------------------------------------
struct TS_TEX2D_DESC : D3D11_TEXTURE2D_DESC
{
    TS_TEX2D_DESC( TsUint width ,
                   TsUint height ,
                   DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM ,
                   D3D11_BIND_FLAG bindflg = D3D11_BIND_SHADER_RESOURCE ,
                   TsUint cpuAccess = 0 ,
                   D3D11_USAGE usage = D3D11_USAGE_DEFAULT ,
                   TsUint mipmapLevel = 1 ,
                   TS_SAMPLE_DESC sample = TS_SAMPLE_DESC(),
                   TsUint miscflg = 0,
                   TsUint arraySize = 1
                   ) 
    {
        Width  = width;
        Height = height;
        BindFlags = bindflg;
        Format = format;
        CPUAccessFlags = cpuAccess;
        Usage = usage;
        MipLevels = mipmapLevel;
        SampleDesc = sample;
        MiscFlags = miscflg;
        ArraySize = arraySize;
    }
};