#include "../TsDefaultShading_Common.hlsli"

float4 main(float4 pos		: SV_POSITION,
            float2 uv : TEXCOORD0,
            float3 normal : TEXCOORD1,
            float4 worldPos : TEXCOORD2) : SV_TARGET
{

    return float4(normal*0.5+0.5f,1);
}