#include "../TsMath.hlsli"
#include "TsSS_common.hlsli"

cbuffer ViewCB : register ( b7 )
{
    float4x4 g_MtxView;
    float4x4 g_MtxProj;
    float4x4 g_MtxInvView;
    float4x4 g_MtxInvProj;
    float4x4 g_MtxVP;

    float4 g_worldCameraPos;

    float g_near;
    float g_far;
    float g_fov;
    float ViewCB_Dumy;
}

//‹…‚ð‹ßŽ—‚µ‚½“_
static float3 g_samples[16] =
{
    float3( 0.53812504 , 0.18565957 , -0.43192 ) ,
    float3( 0.13790712 , 0.24864247 , 0.44301823 ) ,
    float3( 0.33715037 , 0.56794053 , -0.005789503 ) ,
    float3( -0.6999805 , -0.04511441 , -0.0019965635 ) ,
    float3( 0.06896307 , -0.15983082 , -0.85477847 ) ,
    float3( 0.056099437 , 0.006954967 , -0.1843352 ) ,
    float3( -0.014653638 , 0.14027752 , 0.0762037 ) ,
    float3( 0.010019933 , -0.1924225 , -0.034443386 ) ,
    float3( -0.35775623 , -0.5301969 , -0.43581226 ) ,
    float3( -0.3169221 , 0.106360726 , 0.015860917 ) ,
    float3( 0.010350345 , -0.58698344 , 0.0046293875 ) ,
    float3( -0.08972908 , -0.49408212 , 0.3287904 ) ,
    float3( 0.7119986 , -0.0154690035 , -0.09183723 ) ,
    float3( -0.053382345 , 0.059675813 , -0.5411899 ) ,
    float3( 0.035267662 , -0.063188605 , 0.54602677 ) ,
    float3( -0.47761092 , 0.2847911 , -0.0271716 )
};

static float3 g_radius = 7.0f;

float4 main( PS_SS_INPUT_UVx1 In ,
             Texture2D NormalMap : register( t0 ) ,
             Texture2D PositionMap : register( t1 ) ,
             SamplerState samp : register( s0 ) ) : SV_TARGET
{

    float4 normalDepth = NormalMap.Sample( samp , In.uv0 );
    float3 normal = normalDepth.xyz * 2.0 - 1.0f;
    float  depth  = normalDepth.w;
    
    float ao = 0;
    float radD = g_radius / depth;
    float2 se = 0;
    for( int i = 0; i < 16; i++ )
    {
        float3 ray = reflect(g_samples[i].xyz, normal) * radD;
        float2 se = In.uv0 + sign(dot(ray, normal)) * ray * float2(1.0f, -1.0f);

        float4 occNormalDepth = NormalMap.Sample( samp,se );
        float3 occNormal = occNormalDepth.xyz * 2.0 -1.0f;
        float3 occDepth  = occNormalDepth.w;

        float depthDiff     = depth - occDepth;
        float normalDiff = (1.0f - dot(normalize(occNormal), normalize(normal)));

        float falloff = 0.000f;
        float strength = 0.000f;
        ao += step(falloff, depthDiff) * normalDiff * (1.0f - smoothstep(falloff, strength, depthDiff));

    }
    float toStrength = 1.0f;

    ao = 1.0f - toStrength * ao * (1.0f / 16.0f);

    return ao;
}