
#include "../TsDefaultShading_Common.hlsli"
#include "../TsMath.hlsli"
#include "TsSS_common.hlsli"

float PCF8(Texture2D shadowMap,
           SamplerComparisonState ShadowSmp ,
           float3 shadowUV)
{
    float  shadowBias = 0.0008f;
    float cmp = (shadowUV.z - shadowBias);

    float sampleScale = 1.5f;

    //! texelのサイズを求める
    float2 n = 1.0f / GetTextureSize( shadowMap ) * sampleScale;

    float s = shadowMap.SampleCmpLevelZero(
                        ShadowSmp, 
                        shadowUV.xy , 
                        cmp);

    //! 8点をサンプリングする
    s += shadowMap.SampleCmpLevelZero(
                        ShadowSmp, 
                        shadowUV.xy + n * float2(0,1), 
                        cmp);
    s += shadowMap.SampleCmpLevelZero(
                        ShadowSmp, 
                        shadowUV.xy + n * float2(0, -1),
                        cmp);

    s += shadowMap.SampleCmpLevelZero(
                        ShadowSmp, 
                        shadowUV.xy + n * float2(1, 0),
                        cmp);
    s += shadowMap.SampleCmpLevelZero(
                        ShadowSmp, 
                        shadowUV.xy + n * float2(-1, 0),
                        cmp);

    s += shadowMap.SampleCmpLevelZero(
                        ShadowSmp, 
                        shadowUV.xy + n * float2(1, 1),
                        cmp);
    s += shadowMap.SampleCmpLevelZero(
                        ShadowSmp, 
                        shadowUV.xy + n * float2(1, -1),
                        cmp);

    s += shadowMap.SampleCmpLevelZero(
                        ShadowSmp, 
                        shadowUV.xy + n * float2(-1, 1),
                        cmp);
    s += shadowMap.SampleCmpLevelZero(
                        ShadowSmp, 
                        shadowUV.xy + n * float2(-1, -1),
                        cmp);

    s /= 9.0f;

    return s;
}


float4 main( PS_SS_INPUT_UVx1 In ,
             Texture2D DepthNormalMap : register( t0 ) ,
             Texture2D ShadowMap : register( t1 ) ,
             SamplerState linearSample : register( s0 ) ,
             SamplerComparisonState  ShadowSmp : register( s3 ) ):SV_TARGET
{
    float4 depthNormal = DepthNormalMap.Sample( linearSample , In.uv0 );

    float3 normal = depthNormal.xyz * 2.0f - 1.0f;
    float  depth = depthNormal.w;

    // uv空間 → World座標系へ変換
    float4 worldPos = DepthToWorldPos(In.uv0, depth);

    // world座標空間 → Light空間へ変換
    float4x4 shadowMtx = g_LightData[0].worldToShadowMatrix;
    float4 shadowPos = mul( float4(worldPos.xyz,1) , shadowMtx );
    shadowPos.xyz /= shadowPos.w;

    // Light空間　→　uv空間へ変換
    float2 shadowUV = shadowPos.xy;
    shadowUV.x = ( shadowUV.x + 1.0f ) * 0.5f;
    shadowUV.y = ( 1.0f - shadowUV.y ) * 0.5f;

    float s = PCF8(ShadowMap, ShadowSmp, float3(shadowUV, shadowPos.z));
    
    s = lerp(0.5, 1, s);
    float3 dir = g_LightData[0].dir.xyz ;
    float d = max(dot((dir), normalize(normal)),0)*0.5+0.5;

    d = min(d,s);

    return float4(s,s,s, 1);
}
