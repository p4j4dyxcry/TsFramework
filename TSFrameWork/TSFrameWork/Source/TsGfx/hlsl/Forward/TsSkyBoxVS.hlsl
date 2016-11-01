#include "../TsDefaultShading_Common.hlsli"

struct VS_OUT
{
    float4 pos		: SV_POSITION;
    float3 uv		: TEXCOORD0;
};

VS_OUT main(float4 input : SV_POSITION)
{
    VS_OUT output;
    output.pos = mul(input, g_MtxVP);
    output.uv = input - g_worldCameraPos.xyz;
    return output;
}