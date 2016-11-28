#include "../TsDefaultShading_Common.hlsli"
#include "../TsMath.hlsli"
struct PS_IN
{
    float4 pos		: SV_POSITION;
    float2 uv		: TEXCOORD0;
    float3 normal	: TEXCOORD1;
    float4 projPos : TEXCOORD2;
    float4 worldPos : TEXCOORD3;
};

PS_GBUFFER_OUTPUT_3 main( PS_IN input ,
    Texture2D albedoMap  : register(TEX_REGISTER_ALBEDO),
    Texture2D nomalMap   : register( TEX_REGISTER_NORMAL ) ,
    Texture2D speclurMap : register( TEX_REGISTER_SPECULER ) ,
    SamplerState samp    : register(s0))
{
    PS_GBUFFER_OUTPUT_3 output = ( PS_GBUFFER_OUTPUT_3 )0;

    // albedo color
    output.data0 = g_useDiffuseMap >0?
        albedoMap.Sample( samp , input.uv ) :
        float4( max( g_Diffuse.xyz , g_Ambient.xyz ) , g_Diffuse.w );

    // normal to 0 ~ 1
    output.data1.xyz = g_useNomalMap >0 ?
        nomalMap.Sample( samp , input.uv ) :
        PackUnsigned( input.normal );

    // depth
    output.data1.w = input.projPos.z / input.projPos.w;

    // world space uv
    output.data2 = float4(input.uv,0,1);

    output.data2 = float4(input.worldPos.xyz,1);

    return output;
}