#include "TsSS_common.hlsli"

PS_SS_INPUT_UVx1 main(  float4 pos : POSITION ,
						float2 tex : TEXCOORD)
{
	PS_SS_INPUT_UVx1 vsOut;
	vsOut.pos = pos;
	vsOut.uv0 = tex;
	return vsOut;
}