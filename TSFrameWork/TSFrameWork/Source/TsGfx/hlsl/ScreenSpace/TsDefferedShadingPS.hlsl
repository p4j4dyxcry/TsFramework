#include "../TsMath.hlsli"
#include "TsSS_common.hlsli"

float4 main( PS_SS_INPUT_UVx1 input ,
             Texture2D ColorMap		: register( t0 ) ,
             Texture2D NormalMap    : register( t1 ) ,
             Texture2D DepthMap     : register( t2 ) ,
             SamplerState samp      : register( s0 ) ) : SV_TARGET
{

    return float4( input.uv0.r , 1 , 1 , 1 );

    //return tex0.Sample(samp,input.uv0) + 
    //     tex1.Sample(samp,input.uv0);
}