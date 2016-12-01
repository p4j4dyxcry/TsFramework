#include "../TsDefaultShading_Common.hlsli"

float4 main(float4 pos		: SV_POSITION,
            float4 worldPos : TEXCOORD2):SV_TARGET
{
    float z = worldPos.z / worldPos.w;

    return float4(z,z,z,1);
}