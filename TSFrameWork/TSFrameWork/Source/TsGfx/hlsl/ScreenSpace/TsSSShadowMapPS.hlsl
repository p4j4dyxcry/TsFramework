
#include "../TsDefaultShading_Common.hlsli"
#include "../TsMath.hlsli"
#include "TsSS_common.hlsli"

float PCF8(Texture2D shadowMap,
           SamplerComparisonState ShadowSmp ,
           float3 shadowUV)
{

    // �ő�[�x�X�΂����߂�.
    float  maxDepthSlope = max(abs(ddx(shadowUV.z)), abs(ddy(shadowUV.z)));
    
    float  shadowThreshold = 1.0f;      // �V���h�E�ɂ��邩�ǂ�����臒l�ł�.
    float  bias            = 0.0015f;     // �Œ�o�C�A�X�ł�.
    float  slopeScaledBias = 0.01f;     // �[�x�X��.
    float  depthBiasClamp  = 0.1f;      // �o�C�A�X�N�����v�l.
    
    float  shadowBias = bias + slopeScaledBias * maxDepthSlope;
    shadowBias = min( shadowBias, depthBiasClamp );
    float cmp = (shadowUV.z - shadowBias);

    float sampleScale = 4;

    //! texel�̃T�C�Y�����߂�
    float2 n = 1.0f / GetTextureSize( shadowMap ) * sampleScale;

    float s = shadowMap.SampleCmpLevelZero(
                        ShadowSmp, 
                        shadowUV.xy , 
                        cmp);

    //! 8�_���T���v�����O����
    s += shadowMap.SampleCmpLevelZero(
                        ShadowSmp, 
                        shadowUV.xy + n * float2(0,1), 
                        cmp);
    s += shadowMap.SampleCmpLevelZero(
                        ShadowSmp, 
                        shadowUV.xy + n * float2(0, -1),
                        cmp);

    s += shadowMap.SampleCmpLevelZero(
                        ShadowSmp, 
                        shadowUV.xy + n * float2(1, 0),
                        cmp);
    s += shadowMap.SampleCmpLevelZero(
                        ShadowSmp, 
                        shadowUV.xy + n * float2(-1, 0),
                        cmp);

    s += shadowMap.SampleCmpLevelZero(
                        ShadowSmp, 
                        shadowUV.xy + n * float2(1, 1),
                        cmp);
    s += shadowMap.SampleCmpLevelZero(
                        ShadowSmp, 
                        shadowUV.xy + n * float2(1, -1),
                        cmp);

    s += shadowMap.SampleCmpLevelZero(
                        ShadowSmp, 
                        shadowUV.xy + n * float2(-1, 1),
                        cmp);
    s += shadowMap.SampleCmpLevelZero(
                        ShadowSmp, 
                        shadowUV.xy + n * float2(-1, -1),
                        cmp);

    s /= 9.0f;

    return 1-s;
}


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
    float4 worldPos = DepthToWorldPos(In.uv0, depth);

    // world���W��� �� Light��Ԃ֕ϊ�
    float4x4 shadowMtx = g_LightData[0].worldToShadowMatrix;
    float4 shadowPos = mul( float4(worldPos.xyz,1) , shadowMtx );
    shadowPos.xyz /= shadowPos.w;

    // Light��ԁ@���@uv��Ԃ֕ϊ�
    float2 shadowUV = shadowPos.xy;
    shadowUV.x = ( shadowUV.x + 1.0f ) * 0.5f;
    shadowUV.y = ( 1.0f - shadowUV.y ) * 0.5f;

    float s = PCF8(ShadowMap, ShadowSmp, float3(shadowUV, shadowPos.z));

    return float4( s,s,s, 1 );
}
