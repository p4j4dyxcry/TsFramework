//! スクリーンスペース加算合成
#include "../TsMath.hlsli"
#include "TsSS_common.hlsli"



float4 main( PS_SS_INPUT_UVx1 input ,
             Texture2D tex0		: register( t0 ) ,
             Texture2D tex1		: register( t1 ) ,
             SamplerState samp  : register(s0)) : SV_TARGET
{
    float4 a = tex0.Sample(samp, input.uv0);
    float4 b = tex1.Sample(samp, input.uv0);

    // 1 - ( (1-A) * (1-B) )
    return a+b - a*b;
}