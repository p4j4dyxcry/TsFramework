#include "../TsDefaultShading_Common.hlsli"

struct VS_OUT
{
    float4 pos		: SV_POSITION;
    float4 worldPos : TEXCOORD0;
};

VS_OUT main(VS_SKIN_INPUT input)
{
    VS_OUT output;
    LightData lightData = g_LightData[0];
    float4x4 shadowMtx = lightData.worldToShadowMatrix;

    output.worldPos = mul(ComputeSkinMesh(input), g_MtxWorld);
    output.pos = mul(output.worldPos, shadowMtx);
    output.worldPos = output.pos;
    return output;
}