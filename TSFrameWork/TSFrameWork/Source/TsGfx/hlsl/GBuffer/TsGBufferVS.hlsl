#include "../TsDefaultShading_Common.hlsli"

struct VS_OUT
{
    float4 pos		: POSITION;
    float2 uv		: TEXCOORD0;
    float3 normal	: TEXCOORD1;
    float4 screenPos : TEXCOORD2;
    float4 worldPos  : TEXCOORD3;
};

VS_OUT main( VS_SKIN_INPUT input )
{
    VS_OUT output;

    float4 worldPos = mul( ComputeSkinMesh( input ) , g_MtxWorld );

    output.worldPos = worldPos;
    output.screenPos = mul( worldPos , g_MtxVP );
    output.pos = output.screenPos;

    output.normal = mul( ComputeSkinNormal( input ) , ( float3x3 )g_MtxWorld );
    output.uv = input.uv;
    return output;
}