//! ソーベルフィルタ
#include "TsSS_common.hlsli"

static const float  g_KernelNUM = 9;
static float2 g_Kernel[g_KernelNUM] =
{
    float2(-1.0f, -1.0f),
    float2( 0.0f, -1.0f),
    float2( 1.0f, -1.0f),
    float2(-1.0f,  0.0f),
    float2( 0.0f,  0.0f),
    float2( 1.0f,  0.0f),
    float2(-1.0f,  1.0f),
    float2( 0.0f,  1.0f),
    float2( 1.0f,  1.0f)
};

static float g_Weight[g_KernelNUM] =
{
     1.0f,
     1.0f,
     1.0f,
     1.0f,
    -8.0f,
     1.0f,
     1.0f,
     1.0f,
     1.0f
};


float4 main(PS_SS_INPUT_UVx1 input,
    Texture2D tex0		: register(t0),
    SamplerState samp : register(s0)) : SV_TARGET
{
    float2 textureSize = GetTextureSize(tex0);

    float2 invTextureSize = 1.0f / textureSize;;

    float4 Color = 0;

    for (int i = 0; i < g_KernelNUM; i++)
    {
        Color += tex0.Sample(samp, input.uv0 + g_Kernel[i] * invTextureSize)  * g_Weight[i];
    }

    return Color;
}