
#include "../TsDefaultShading_Common.hlsli"
#include "../TsMath.hlsli"
#include "TsSS_common.hlsli"

float4 main( PS_SS_INPUT_UVx1 In ,
             Texture2D DepthNormalMap : register( t0 ) ,
             Texture2D ShadowMap : register( t1 ) ,
             SamplerState linearSample : register( s0 ) ,
             SamplerComparisonState  ShadowSmp : register( s3 ) ):SV_TARGET
{
    float4 depthNormal = DepthNormalMap.Sample( linearSample , In.uv0 );

    float3 normal = depthNormal.xyz * 2.0f - 1.0f;
    float  depth = depthNormal.w;

    // uv��� �� World���W�n�֕ϊ�
    float2 projPos = float2( In.uv0.x * 2 - 1.0f, (1 - In.uv0.y ) * 2 - 1 );
    float4 viewPos = mul( float4( projPos , depth , 1 ) , g_MtxInvProj );
    float4 worldPos = mul( viewPos , g_MtxInvView );
    worldPos.xyz/=worldPos.w;

    // world���W��� �� Light��Ԃ֕ϊ�
    float4x4 shadowMtx = g_LightData[0].worldToShadowMatrix;
    float4 shadowPos = mul( float4(worldPos.xyz,1) , shadowMtx );
    shadowPos.xyz /= shadowPos.w;
    // Light��ԁ@���@uv��Ԃ֕ϊ�
    float2 shadowUV = shadowPos.xy;
//        shadowUV = shadowUV * 0.5 + 0.5;
    shadowUV.x = ( shadowUV.x + 1.0f ) / 2.0f;
    shadowUV.y = ( (1.0f - shadowUV.y) ) / 2.0f;

    float lightDepth = ShadowMap.Sample( linearSample , shadowUV ).r;
    //return float4( lightDepth - shadowPos.z, 0 , 0 , 1 );

    float m = ShadowMap.Sample( linearSample , In.uv0 ).r;

    // �ő�[�x�X�΂����߂�.
    float  maxDepthSlope = max( abs( ddx( shadowPos.z ) ) , abs( ddy( shadowPos.z ) ) );

    float  shadowThreshold = 1.0f;      // �V���h�E�ɂ��邩�ǂ�����臒l�ł�.
    float  bias = 0.01f;     // �Œ�o�C�A�X�ł�.
    float  slopeScaledBias = 0.01f;     // �[�x�X��.
    float  depthBiasClamp = 0.1f;      // �o�C�A�X�N�����v�l.

    float  shadowBias = bias + slopeScaledBias * maxDepthSlope;
    shadowBias = min( shadowBias , depthBiasClamp );

    float s = lightDepth > shadowPos.z - shadowBias ? 0 : 1;
    s = lerp( 0.5 , 1 , s );
    float3 dir = g_LightData[0].dir.xyz;
    float temp = dir.z;
    dir.z = dir.x;
    dir.x = temp;
    float d = dot( normal , dir );
    d = d*0.5 + 0.5;
    d *= s;
    return float4( d,d,m, 1 );
}