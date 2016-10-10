#include "../TsDefaultShading_Common.hlsli"

struct VS_OUT
{
    float4 pos		: SV_POSITION;
    float2 uv		: TEXCOORD0;
    float3 normal	: TEXCOORD1;
    float4 worldPos : TEXCOORD2;
};

float4 ComputeSkinMesh( VS_SKIN_INPUT v )
{
    if( v.weight[0] + v.weight[1] + v.weight[2] + v.weight[3] < 0.05f )
        return float4( v.pos , 1 );

    float4 pos = float4(v.pos,1);

    float4 bpos0 = v.weight[0] * mul( pos , g_BoneMatrix[v.boneIdx.x] );
    float4 bpos1 = v.weight[1] * mul( pos , g_BoneMatrix[v.boneIdx.y] );
    float4 bpos2 = v.weight[2] * mul( pos , g_BoneMatrix[v.boneIdx.z] );
    float4 bpos3 = v.weight[3] * mul( pos , g_BoneMatrix[v.boneIdx.w] );
    
    pos = bpos0 + bpos1 + bpos2 + bpos3;

    return pos;
}

VS_OUT main( VS_SKIN_INPUT input )
{
    VS_OUT output;
    output.worldPos = mul( ComputeSkinMesh( input ) , g_MtxWorld );
    output.pos = mul(output.worldPos, g_MtxVP);
    output.normal = mul( input.normal , ( float3x3 )g_MtxWorld );
    output.uv = input.uv;
    return output;
}