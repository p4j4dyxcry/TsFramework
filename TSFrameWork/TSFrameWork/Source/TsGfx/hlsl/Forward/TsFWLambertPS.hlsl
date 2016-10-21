#include "../TsDefaultShading_Common.hlsli"

float4 main( float4 pos		: SV_POSITION,
             float2 uv		: TEXCOORD0,
             float3 normal	: TEXCOORD1,
             float4 worldPos : TEXCOORD2,
             Texture2D albedo : register(t0),
             SamplerState samp: register(s0)) : SV_TARGET
{
    float3 dir = float3( 0 , 1 , 1 );
    dir = normalize( dir );
    
    //albedo 
    float4 color = albedo.Sample( samp , uv ).rgba;

    //rambert
    float d = max(dot(normal, dir), 0.1f);
    d = d* 0.5f + 0.5f;
    //compute CameraVector
    float3 eye = normalize(g_worldCameraPos - worldPos);

    //compute View And Camera HerfVector
    float3 VL = normalize(dir + eye);

    //fong
    float s = pow(max(dot(normal,VL),0),80.0f);

    //rimlight
    float rim = pow(1-dot(eye, normal), 12.5f);

    float4 result = ( d * color + s + rim );
    result = max( 0 , result );
    result = min( 1 , result );

    return result;
}