#include "TsSS_common.hlsli"

PS_SS_INPUT_UVx1 main(  float4 pos : POSITION )
{
    PS_SS_INPUT_UVx1 vsOut;
    vsOut.pos = float4(pos.xy,0,1);
    vsOut.uv0 = pos.zw;
    return vsOut;
}