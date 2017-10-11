#include "../TsDefaultShading_Common.hlsli"

float4 main(float4 pos		: SV_POSITION,
            float4 worldPos : TEXCOORD2):SV_TARGET
{
    float z = worldPos.z / worldPos.w;

    float dx = ddx( z );
    float dy = ddy( z );
    float zz = (z * z);
    //+(0.25 * (dx * dx + dy * dy));

    return float4(z,zz,0,1);
}