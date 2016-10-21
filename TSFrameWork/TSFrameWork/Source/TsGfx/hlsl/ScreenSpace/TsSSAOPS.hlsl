#include "../TsMath.hlsli"
#include "TsSS_common.hlsli"

float Rand( float2 texCoord , int Seed )
{
    return frac( sin( dot( texCoord.xy , float2( 12.9898 , 78.233 ) ) + Seed ) * 43758.5453 );
}

float3 Rand3( float2 texCoord , int Seed )
{
    float3 result;
    result.x = Rand( texCoord , 22 * Seed );
    result.y = Rand( -texCoord , 61 * -Seed );
    result.z = Rand( float2( texCoord.y , texCoord.x ) , 928 * Seed );

    return result;
}

float4 main( PS_SS_INPUT_UVx1 In ,
             Texture2D NormalMap : register( t0 ) ,
             Texture2D DepthMap : register( t1 ) ,
             SamplerState samp : register( s0 ) ) : SV_TARGET
{
    //モンテカルロ用のランダムな点
    float3 samples[16] = 
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

    float3 fres      = Rand3( In.uv0 , 4513 );
    float3 normal   = NormalMap.Sample( samp , In.uv0 ).xyz;
    float depth     = DepthMap.Sample( samp , In.uv0).x;

    float density = 10.0f;
    float strength = 0.01f;
    float falloff = 0.000001f;
    float rad = 0.1f;
    float bl = 0.0;
    float radD = rad / depth;

    float3 ray , occNorm;
    float2 se;
    float occluderDepth;
    float depthDifference;
    float normDiff;
    float	invSamples = 1.0 / 16.0f;
    float	finalColor = 0.0f;

    for( int i = 0; i < 16; i++ )
    {
        ray = radD * reflect( samples[i] , fres );
        se = In.uv0 + sign( dot( ray , normal ) ) * ray * float2( 1.0f , -1.0f );

        float4 occluderFragment = NormalMap.Sample( samp, se );

        occNorm = normalize( occluderFragment.xyz * 2.0f - 1.0f );

        depthDifference = depth - DepthMap.Sample( samp , se ).r;
        normDiff = 1.0 - 
            ( occNorm.x*normal.x + 
              occNorm.y*normal.y + 
              occNorm.z*normal.z );

        bl += step( falloff , depth ) *normDiff * ( 1.0 - smoothstep( falloff , strength , depth ) );

    }

    float ao = 1.0 - density * bl * invSamples;

	return float4(ao,ao,ao,1);
}