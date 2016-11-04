#include "../TsMath.hlsli"
#include "TsSS_common.hlsli"

static const int g_cKernelSize = 13;

static float2 g_PixelKernel[g_cKernelSize] =
{
    { 0, -6 },
    { 0, -5 },
    { 0, -4 },
    { 0, -3 },
    { 0, -2 },
    { 0, -1 },
    { 0, 0 },
    { 0, 1 },
    { 0, 2 },
    { 0, 3 },
    { 0, 4 },
    { 0, 5 },
    { 0, 6 },
};

static float g_BlurWeights[g_cKernelSize] =
{
    0.002216,
    0.008764,
    0.026995,
    0.064759,
    0.120985,
    0.176033,
    0.199471,
    0.176033,
    0.120985,
    0.064759,
    0.026995,
    0.008764,
    0.002216,
};

float4 main(    PS_SS_INPUT_UVx1 input, 
                Texture2D tex0      : register(t0) , 
                SamplerState samp   : register(s0)) : SV_TARGET
{
    float2 textureSize = GetTextureSize(tex0);

    float2 invTextureSize = 1.0f / textureSize;

    float4 Color = 0;

    for (int i = 0; i < g_cKernelSize; i++)
    {
        Color += tex0.Sample(samp, input.uv0 + g_PixelKernel[i] * invTextureSize.y) * g_BlurWeights[i];
    }

    return Color;
}