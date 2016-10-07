//! エッジフィルタ
#include "TsSS_common.hlsli"

float4 main( PS_SS_INPUT_UVx1 input ,
             Texture2D tex0		: register(t0),
             SamplerState samp  : register(s0)) : SV_TARGET
{
    //todo
    return tex0.Sample( samp , input.uv0 );
}