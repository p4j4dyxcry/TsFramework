#include "../TsDefaultShading_Common.hlsli"
#include "../TsMath.hlsli"
struct PS_IN
{
    float4 pos		: SV_POSITION;
    float2 uv		: TEXCOORD0;
    float3 normal	: TEXCOORD1;
    float4 worldPos : TEXCOORD2;
};

PS_GBUFFER_OUTPUT_3 main( PS_IN input ,
    Texture2D tex : register(TEX_REGISTER_ALBEDO),
                          SamplerState samp  : register(s0))
{
    PS_GBUFFER_OUTPUT_3 output = ( PS_GBUFFER_OUTPUT_3 )0;

    // albedo color
    output.data0	 = tex.Sample( samp , input.uv );

    // normal to 0 ~ 1
    output.data1.xyz = PackUnsigned( input.normal );

    // depth
    output.data1.w = input.worldPos.z / input.worldPos.w;

    // world space uv
    output.data2.xy  = input.uv;

    return output;
}