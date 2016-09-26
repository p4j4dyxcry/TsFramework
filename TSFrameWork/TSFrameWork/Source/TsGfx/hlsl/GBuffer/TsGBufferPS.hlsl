#include "../TsDefaultShading_Common.hlsli"
#include "../TsMath.hlsli"
struct PS_IN
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float3 normal	: NORMAL;
};

PS_GBUFFER_OUTPUT_2 main( PS_IN input ,
						  Texture2D tex : register(t0),
						  SamplerState samp  : register(s0))
{
	PS_GBUFFER_OUTPUT_2 output = ( PS_GBUFFER_OUTPUT_2 )0;

	output.data0	 = tex.Sample( samp , input.uv );
	output.data1.xyz = PackUnsigned( input.normal );

	return output;
}