#include "../TsDefaultShading_Common.hlsli"

struct VS_OUT
{
    float4 pos      : SV_POSITION;
    float2 uv       : TEXCOORD0;
    float3 normal   : NORMAL;
};

VS_OUT main( VS_DEFAULT_INPUT input ) 
{
    VS_OUT output;

    output.pos = mul( float4( input.pos , 1 ) , g_MtxWorld );
    output.pos = mul( output.pos , g_MtxVP );
    output.normal = mul( input.normal , ( float3x3 )g_MtxWorld );
    output.uv = input.uv;

    return output;
}