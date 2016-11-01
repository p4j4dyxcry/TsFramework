struct VS_OUT
{
    float4 pos		: SV_POSITION;
    float3 uv		: TEXCOORD0;
};

float4 main( VS_OUT input ,
             TextureCube cubemap : register(t0),
             SamplerState samp   : register(s)) : SV_TARGET
{    
    return cubemap.Sample(samp, normalize(input.uv));
}