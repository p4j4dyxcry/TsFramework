#include "../TsDefaultShading_Common.hlsli"

float4 main( float4 pos		: SV_POSITION,
             float2 uv		: TEXCOORD0,
             float3 normal	: TEXCOORD1,
             float4 worldPos : TEXCOORD2) : SV_TARGET
{
    float3 dir = float3( 1 , -1 , -1 );
    dir = normalize( dir );

    //rambert
    float d = max(dot(normal, dir), 0.1f);

    //compute CameraVector
    float3 eye = normalize(g_worldCameraPos - worldPos);

    //compute View And Camera HerfVector
    float3 VL = normalize(dir + eye);

    //fong
    float s = pow(max(dot(normal,VL),0),12.5f);

    //rimlight
    float rim = pow(1-dot(eye, normal), 4.5f);
    return d + s + rim;
}