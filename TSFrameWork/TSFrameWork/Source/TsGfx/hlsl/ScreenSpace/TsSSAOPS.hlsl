#include "../TsMath.hlsli"
#include "../TsDefaultShading_Common.hlsli"
#include "TsSS_common.hlsli"

//‹…‚ð‹ßŽ—‚µ‚½“_
static float3 g_samples32[32] =
{
    float3(0.53812504, 0.18565957, -0.43192),
    float3(0.13790712, 0.24864247, 0.44301823),
    float3(0.33715037, 0.56794053, -0.005789503),
    float3(-0.6999805, -0.04511441, -0.0019965635),

    float3(0.06896307, -0.15983082, -0.85477847),
    float3(0.056099437, 0.006954967, -0.1843352),
    float3(-0.014653638, 0.14027752, 0.0762037),
    float3(0.010019933, -0.1924225, -0.034443386),

    float3(-0.35775623, -0.5301969, -0.43581226),
    float3(-0.3169221, 0.106360726, 0.015860917),
    float3(0.010350345, -0.58698344, 0.0046293875),
    float3(-0.08972908, -0.49408212, 0.3287904),

    float3(0.7119986, -0.0154690035, -0.09183723),
    float3(-0.053382345, 0.059675813, -0.5411899),
    float3(0.035267662, -0.063188605, 0.54602677),
    float3(-0.47761092, 0.2847911, -0.0271716),

    float3(-0.13657719, 0.30651027, 0.16118456),
    float3(-0.14714938, 0.33245975, -0.113095455),
    float3(0.030659059, 0.27887347, -0.7332209),
    float3(0.009913514, -0.89884496, 0.07381549),

    float3(0.040318526, 0.40091, 0.6847858),
    float3(0.22311053, -0.3039437, -0.19340435),
    float3(0.36235332, 0.21894878, -0.05407306),
    float3(-0.15198798, -0.38409665, -0.46785462),

    float3(-0.013492276, -0.5345803, 0.11307949),
    float3(-0.4972847, 0.037064247, -0.4381323),
    float3(-0.024175806, -0.008928787, 0.17719103),
    float3(0.694014, -0.122672155, 0.33098832),

    float3(0.25109035, -0.1787317, -0.011580509),
    float3(-0.08781511, 0.44514698, 0.56647956),
    float3(-0.011737816, -0.0643377, 0.16030222),
    float3(0.035941467, 0.04990871, -0.46533614)
};
static const int    SAMPLE_NUM = 16;
static const float  INV_SAMPLE = 1.0f / (float)SAMPLE_NUM;
static const float  g_radius = 1.5f;

float4 main( PS_SS_INPUT_UVx1 In ,
             Texture2D NormalMap : register( t0 ) ,
             SamplerState samp : register( s0 ) ) : SV_TARGET
{

    float4 normalDepth = NormalMap.Sample(samp, In.uv0);
    float3 normal = normalDepth.xyz * 2.0 - 1.0f;

    //todo Š®‘S‚Èƒ‰ƒ“ƒ_ƒ€‚É‚µ‚½‚¢
    float random = Rand(In.uv0);
    float3 rand3 = float3(random, Rand(float2(In.uv0.y, random)), Rand(float2(random, Rand(float2(random, In.uv0.y)))));

    float  depth  = normalDepth.w;
    depth = DepthToLinear(depth) ;
    float ao = 0;
    float radD = g_radius / depth;
    float2 se = 0;
        float2 set = 0;
    float a = 0;
    for (int i = 0; i < SAMPLE_NUM; i++)
    {
        float3 ray = reflect(g_samples32[i].xyz, rand3 ) * radD;
        float2 se = In.uv0 + sign(dot(ray, normal)) * ray.xy * float2(1.0f, -1.0f);
        set +=se;
        float4 occNormalDepth = NormalMap.Sample( samp,se );
        float3 occNormal = occNormalDepth.xyz * 2.0 -1.0f;
        float  occDepth  = occNormalDepth.w;
        occDepth = DepthToLinear(occDepth) ;
        float depthDiff     = depth - occDepth;
        float normalDiff = (1.0f - dot(normalize(occNormal), normalize(normal)));

        float falloff = 0.000001f;
        float strength = INV_SAMPLE;
        a += depthDiff;
        ao += step(falloff, depthDiff) * normalDiff * (1.0f - smoothstep(falloff, strength, depthDiff));

    }
    float toStrength = 6.0;

    ao = 1.0f - toStrength * ao * INV_SAMPLE;

    return float4(ao,ao,ao,1);
}