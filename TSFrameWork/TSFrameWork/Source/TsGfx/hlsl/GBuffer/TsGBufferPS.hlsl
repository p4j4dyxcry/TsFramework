#include "../TsDefaultShading_Common.hlsli"
#include "../TsMath.hlsli"
struct PS_IN
{
    float4 pos		: SV_POSITION;
    float2 uv		: TEXCOORD0;
    float3 normal	: TEXCOORD1;
    float4 worldPos : TEXCOORD2;
};

PS_GBUFFER_OUTPUT_4 main( PS_IN input ,
                          Texture2D tex : register(t0),
                          SamplerState samp  : register(s0))
{
    PS_GBUFFER_OUTPUT_4 output = ( PS_GBUFFER_OUTPUT_4 )0;

    // albedo color
    output.data0	 = tex.Sample( samp , input.uv );

    // normal to 0 ~ 1
    output.data1.xyz = PackUnsigned( input.normal );

    // world space uv
    output.data2.xy  = input.uv;

    //proj pos
    output.data3     = mul(input.worldPos,g_MtxVP);

    // depth
    output.data1.w = output.data3.z / output.data3.w;
    output.data3.xyz /= output.data3.w;
    output.data3.xyz = output.data3.xyz *0.5 + 0.5f;;
    return output;
}