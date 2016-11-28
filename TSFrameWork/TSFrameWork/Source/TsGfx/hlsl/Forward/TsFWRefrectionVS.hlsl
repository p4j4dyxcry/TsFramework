#include "../TsDefaultShading_Common.hlsli"

struct VS_OUT
{
    float4 pos		: SV_POSITION;
    float2 uv		: TEXCOORD0;
    float3 normal	: TEXCOORD1;
    float4 worldPos : TEXCOORD2;
};

VS_OUT main( VS_SKIN_INPUT input )
{
    VS_OUT output;

    output.worldPos = mul( ComputeSkinMesh( input ) , g_MtxWorld );
    output.pos = mul( output.worldPos , g_MtxVP );
    output.normal = mul( ComputeSkinNormal( input ) , ( float3x3 )g_MtxWorld );
    output.uv = input.uv;
    return output;
}