//! スクリーンスペース加算合成
#include "../TsMath.hlsli"
#include "TsSS_common.hlsli"



float4 main( PS_SS_INPUT_UVx1 input ,
             Texture2D tex0		: register( t0 ) ,
             Texture2D tex1		: register( t1 ) ,
             SamplerState samp  : register(s0)) : SV_TARGET
{

    return float4( input.uv0.r , 1 , 1 , 1 );

    //return tex0.Sample(samp,input.uv0) + 
    //     tex1.Sample(samp,input.uv0);
}