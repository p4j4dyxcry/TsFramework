//!*******************************************************
//! TsShaderUT.h
//!
//! Shader Utility.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

enum TS_SHADER_TYPE 
{
    UNKNOWN_SHADER = 0x00 ,

    VERTEX_SHADER = 0x01 ,
    PIXEL_SHADER = 0x02 ,
    GEOMETRY_SHADER = 0x04 ,
    HULL_SHADER = 0x08 ,
    DOMAIN_SHADER = 0x10 ,
    COMPUTE_SHADER = 0x20 ,

    // vertex & pixel
    VP_SHADER = 0x03 ,
    ALL_SHADER = 0x3f ,
};

enum TS_SHADER_INDEX 
{
    VS_IDX = 0x00,
    PS_IDX = 0x01,
    GS_IDX = 0x02,
    HS_IDX = 0x03,
    DS_IDX = 0x04,
    CS_IDX = 0x05,

    SHADER_MAX_IDX = 0x06,
};

enum TS_CBUFFER_REGISTER 
{
    ObjectMatrixCB = 0,
    SkinBoneCB = 1,       // used Vertex Shader
    LightSetCB = 2,
    MaterialB = 3,   // used Pixel Shader;
    ViewCB = 7,
};


namespace TSShaderUT
{
    enum class VARIABLE_TYPE : TsUint
    {
        FLOAT,
        INT,
        BOOL,
        MATRIX,
        UNKNOWN,
        NUM
    };

    static const TsChar * VARIABLE_STRING[(TsUint)VARIABLE_TYPE::NUM] =
    {
        "Float",
        "Int",
        "Bool",
        "Matrix",
        "Unknown"
    };

    inline TsString TsComvertShaderType( TS_SHADER_TYPE shaderType )
    {
        switch( shaderType )
        {
            case TS_SHADER_TYPE::VERTEX_SHADER:return "VetexShader";
            case TS_SHADER_TYPE::PIXEL_SHADER:return "PixlShader";
            case TS_SHADER_TYPE::GEOMETRY_SHADER:return "GiometoryShader";
            case TS_SHADER_TYPE::HULL_SHADER:return "HullShader";
            case TS_SHADER_TYPE::DOMAIN_SHADER:return "DomainShader";
            default:
            case TS_SHADER_TYPE::UNKNOWN_SHADER: return "UnknownShader";
        }
    }

    inline TsBool IsVS( TS_SHADER_TYPE type )
    {
        return ( ( TsUint )type & ( TsUint )TS_SHADER_TYPE::VERTEX_SHADER ) > 0;
    }

    inline TsBool IsPS( TS_SHADER_TYPE type )
    {
        return ( ( TsUint )type & ( TsUint )TS_SHADER_TYPE::PIXEL_SHADER ) > 0;
    }

    inline TsBool IsGS( TS_SHADER_TYPE type )
    {
        return ( ( TsUint )type & ( TsUint )TS_SHADER_TYPE::GEOMETRY_SHADER ) > 0;
    }

    inline TsBool IsHS( TS_SHADER_TYPE type )
    {
        return ( ( TsUint )type & ( TsUint )TS_SHADER_TYPE::HULL_SHADER ) > 0;
    }

    inline TsBool IsDS( TS_SHADER_TYPE type )
    {
        return ( ( TsUint )type & ( TsUint )TS_SHADER_TYPE::DOMAIN_SHADER ) > 0;
    }

    inline TsBool IsCS( TS_SHADER_TYPE type )
    {
        return ( ( TsUint )type & ( TsUint )TS_SHADER_TYPE::COMPUTE_SHADER ) > 0;
    }

    inline DXGI_FORMAT ComvertTextureFormat(const TsString& fmt)
    {
        if (fmt == "Unknown")
            return DXGI_FORMAT_UNKNOWN;

        if (fmt == "TYPELESS_R32G32B32A32")
            return DXGI_FORMAT_R32G32B32A32_TYPELESS;
        if (fmt == "FLOAT_R32G32B32A32")
            return DXGI_FORMAT_R32G32B32A32_FLOAT;
        if (fmt == "UINT_R32G32B32A32")
            return DXGI_FORMAT_R32G32B32A32_UINT;
        if (fmt == "SINT_R32G32B32A32")
            return DXGI_FORMAT_R32G32B32A32_SINT;

        if (fmt == "TYPELESS_R32G32B32")
            return DXGI_FORMAT_R32G32B32_TYPELESS;
        if (fmt == "FLOAT_R32G32B32")
            return DXGI_FORMAT_R32G32B32_FLOAT;
        if (fmt == "UINT_R32G32B32")
            return DXGI_FORMAT_R32G32B32_UINT;
        if (fmt == "SINT_R32G32B32")
            return DXGI_FORMAT_R32G32B32_SINT;

        if (fmt == "TYPELESS_R16G16B16A16")
            return DXGI_FORMAT_R16G16B16A16_TYPELESS;
        if (fmt == "FLOAT_R16G16B16A16")
            return DXGI_FORMAT_R16G16B16A16_FLOAT;
        if (fmt == "UNORM_R16G16B16A16")
            return DXGI_FORMAT_R16G16B16A16_UNORM;
        if (fmt == "UINT_R16G16B16A16")
            return DXGI_FORMAT_R16G16B16A16_UINT;
        if (fmt == "SINT_R16G16B16A16")
            return DXGI_FORMAT_R16G16B16A16_SINT;


        if (fmt == "TYPELESS_R32G32")
            return DXGI_FORMAT_R32G32_TYPELESS;
        if (fmt == "FLOAT_R32G32")
            return DXGI_FORMAT_R32G32_FLOAT;
        if (fmt == "UINT_R32G32")
            return DXGI_FORMAT_R32G32_UINT;
        if (fmt == "SINT_R32G32")
            return DXGI_FORMAT_R32G32_SINT;

        if (fmt == "TYPELESS_R32G8X24")
            return DXGI_FORMAT_R32G8X24_TYPELESS;
        if (fmt == "FLOAT_D32_UINT_S8X24")
            return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        if (fmt == "FLOAT_R32_TYPELESS_X8X24")
            return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
        if (fmt == "TYPELESS_X32_UINT_G8X24")
            return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;

        if (fmt == "TYPELESS_R10G10B10A2")
            return DXGI_FORMAT_R10G10B10A2_TYPELESS;
        if (fmt == "UNORM_R10G10B10A2")
            return DXGI_FORMAT_R10G10B10A2_UNORM;
        if (fmt == "UINT_TYPELESS_R10G10B10A2")
            return DXGI_FORMAT_R10G10B10A2_UINT;
        if (fmt == "FLOAT_TYPELESS_R11G11B10")
            return DXGI_FORMAT_R11G11B10_FLOAT;


        if (fmt == "TYPELESS_R8G8B8A8")
            return DXGI_FORMAT_R8G8B8A8_TYPELESS;
        if (fmt == "UNORM_R8G8B8A8")
            return DXGI_FORMAT_R8G8B8A8_UNORM;
        if (fmt == "UNORM_SRGB_R8G8B8A8")
            return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        if (fmt == "UINT_R8G8B8A8")
            return DXGI_FORMAT_R8G8B8A8_UINT;
        if (fmt == "SINT_R8G8B8A8")
            return DXGI_FORMAT_R8G8B8A8_SINT;

        if (fmt == "TYPELESS_R16G16")
            return DXGI_FORMAT_R16G16_TYPELESS;
        if (fmt == "FLOAT_R16G16")
            return DXGI_FORMAT_R16G16_FLOAT;
        if (fmt == "UNORM_R16G16")
            return DXGI_FORMAT_R16G16_UNORM;
        if (fmt == "UINT_R16G16")
            return DXGI_FORMAT_R16G16_UINT;
        if (fmt == "SNORM_R16G16")
            return DXGI_FORMAT_R16G16_SNORM;
        if (fmt == "SINT_R16G16")
            return DXGI_FORMAT_R16G16_SINT;

        if (fmt == "TYPELESS_R32")
            return DXGI_FORMAT_R32_TYPELESS;
        if (fmt == "FLOAT_D32")
            return DXGI_FORMAT_D32_FLOAT;
        if (fmt == "FLOAT_R32")
            return DXGI_FORMAT_R32_FLOAT;
        if (fmt == "UINT_R32")
            return DXGI_FORMAT_R32_UINT;
        if (fmt == "SINT_R32")
            return DXGI_FORMAT_R32_SINT;

        if (fmt == "TYPELESS_R24G8")
            return DXGI_FORMAT_R24G8_TYPELESS;
        if (fmt == "UNORM_D24_UINT_S8")
            return DXGI_FORMAT_D24_UNORM_S8_UINT;
        if (fmt == "UNORM_R24_TYPELESS_X8")
            return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        if (fmt == "TYPELESS_X24_UINT_G8")
            return DXGI_FORMAT_X24_TYPELESS_G8_UINT;

        if (fmt == "TYPELESS_R8G8")
            return DXGI_FORMAT_R8G8_TYPELESS;
        if (fmt == "UNORM_R8G8")
            return DXGI_FORMAT_R8G8_UNORM;
        if (fmt == "UINT_R8G8")
            return DXGI_FORMAT_R8G8_UINT;
        if (fmt == "SNORM_R8G8")
            return DXGI_FORMAT_R8G8_SNORM;
        if (fmt == "SINT_R8G8")
            return DXGI_FORMAT_R8G8_SINT;

        if (fmt == "TYPELESS_R16")
            return DXGI_FORMAT_R16_TYPELESS;
        if (fmt == "FLOAT_R16")
            return DXGI_FORMAT_R16_FLOAT;
        if (fmt == "UNORM_D16")
            return DXGI_FORMAT_D16_UNORM;
        if (fmt == "UNORM_R16")
            return DXGI_FORMAT_R16_UNORM;
        if (fmt == "UINT_R16")
            return DXGI_FORMAT_R16_UINT;
        if (fmt == "SNORM_R16")
            return DXGI_FORMAT_R16_SNORM;
        if (fmt == "SINT_R16")
            return DXGI_FORMAT_R16_SINT;

        if (fmt == "TYPELESS_R8")
            return DXGI_FORMAT_R8_TYPELESS;
        if (fmt == "UNORM_R8")
            return DXGI_FORMAT_R8_UNORM;
        if (fmt == "UINT_R8")
            return DXGI_FORMAT_R8_UINT;
        if (fmt == "SNORM_R8")
            return DXGI_FORMAT_R8_SNORM;
        if (fmt == "SINT_R8")
            return DXGI_FORMAT_R8_SINT;
        if (fmt == "UNORM_A8")
            return DXGI_FORMAT_A8_UNORM;
        if (fmt == "UNORM_R1")
            return DXGI_FORMAT_R1_UNORM;

        if (fmt == "SHAREDEXP_R9G9B9E5")
            return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
        if (fmt == "UNORM_R8G8B8G8")
            return DXGI_FORMAT_R8G8_B8G8_UNORM;
        if (fmt == "UNORM_G8R8G8B8")
            return DXGI_FORMAT_G8R8_G8B8_UNORM;

        if (fmt == "TYPELESS_BC1")
            return DXGI_FORMAT_BC1_TYPELESS;
        if (fmt == "UNORM_BC1")
            return DXGI_FORMAT_BC1_UNORM;
        if (fmt == "UNORM_SRGB_BC1")
            return DXGI_FORMAT_BC1_UNORM_SRGB;

        if (fmt == "TYPELESS_BC2")
            return DXGI_FORMAT_BC2_TYPELESS;
        if (fmt == "UNORM_BC2")
            return DXGI_FORMAT_BC2_UNORM;
        if (fmt == "UNORM_SRGB_BC2")
            return DXGI_FORMAT_BC2_UNORM_SRGB;

        if (fmt == "TYPELESS_BC3")
            return DXGI_FORMAT_BC3_TYPELESS;
        if (fmt == "UNORM_BC3")
            return DXGI_FORMAT_BC3_UNORM;
        if (fmt == "UNORM_SRGB_BC3")
            return DXGI_FORMAT_BC3_UNORM_SRGB;

        if (fmt == "TYPELESS_BC4")
            return DXGI_FORMAT_BC4_TYPELESS;
        if (fmt == "UNORM_BC4")
            return DXGI_FORMAT_BC4_UNORM;
        if (fmt == "SNORM_BC4")
            return DXGI_FORMAT_BC4_SNORM;

        if (fmt == "TYPELESS_BC5")
            return DXGI_FORMAT_BC5_TYPELESS;
        if (fmt == "UNORM_BC5")
            return DXGI_FORMAT_BC5_UNORM;
        if (fmt == "SNORM_BC5")
            return DXGI_FORMAT_BC5_SNORM;

        if (fmt == "UNORM_B5G6R5")
            return DXGI_FORMAT_B5G6R5_UNORM;
        if (fmt == "UNORM_B5G5R5A1")
            return DXGI_FORMAT_B5G5R5A1_UNORM;
        if (fmt == "UNORM_B8G8R8A8")
            return DXGI_FORMAT_B8G8R8A8_UNORM;
        if (fmt == "UNORM_B8G8R8X8")
            return DXGI_FORMAT_B8G8R8X8_UNORM;

        if (fmt == "TYPELESS_B8G8R8A8")
            return DXGI_FORMAT_B8G8R8A8_TYPELESS;
        if (fmt == "UNORM_SRGB_B8G8R8X8")
            return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
        if (fmt == "TYPELESS_B8G8R8X8")
            return DXGI_FORMAT_B8G8R8X8_TYPELESS;
        if (fmt == "UNORM_SRGB_B8G8R8X8")
            return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;

        if (fmt == "TYPELESS_BC6H")
            return DXGI_FORMAT_BC6H_TYPELESS;
        if (fmt == "UF16_BC6H")
            return DXGI_FORMAT_BC6H_UF16;
        if (fmt == "SF16_BC6H")
            return DXGI_FORMAT_BC6H_SF16;

        if (fmt == "TYPELESS_BC7H")
            return DXGI_FORMAT_BC7_TYPELESS;
        if (fmt == "UNORM_BC7H")
            return DXGI_FORMAT_BC7_UNORM;
        if (fmt == "UNORM_SRGB_BC6H")
            return DXGI_FORMAT_BC6H_SF16;

        if (fmt == "AYUV")
            return DXGI_FORMAT_AYUV;
        if (fmt == "Y410")
            return DXGI_FORMAT_Y410;
        if (fmt == "Y416")
            return DXGI_FORMAT_Y416;
        if (fmt == "NV12")
            return DXGI_FORMAT_NV12;
        if (fmt == "P010")
            return DXGI_FORMAT_P010;
        if (fmt == "P016")
            return DXGI_FORMAT_P016;
        if (fmt == "420_OPAQUE")
            return DXGI_FORMAT_420_OPAQUE;
        if (fmt == "YUY2")
            return DXGI_FORMAT_YUY2;
        if (fmt == "Y210")
            return DXGI_FORMAT_Y210;
        if (fmt == "Y216")
            return DXGI_FORMAT_Y216;
        if (fmt == "NV11")
            return DXGI_FORMAT_NV11;
        if (fmt == "AI44")
            return DXGI_FORMAT_AI44;
        if (fmt == "P8")
            return DXGI_FORMAT_P8;
        if (fmt == "A8P8")
            return DXGI_FORMAT_A8P8;
        if (fmt == "U_NORM_B4G4R4A4")
            return DXGI_FORMAT_B4G4R4A4_UNORM;

        TsDebugLog("未対応のフォーマット\n")
            return DXGI_FORMAT_BC7_UNORM_SRGB;
    }


}

