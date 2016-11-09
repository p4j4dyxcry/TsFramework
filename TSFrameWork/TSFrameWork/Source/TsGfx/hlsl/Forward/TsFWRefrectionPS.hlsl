#include "../TsDefaultShading_Common.hlsli"
#include "../TsMath.hlsli"

float3 toGamma(float3 lin)
{
    return pow(lin.rgb, 1.0f / 2.2);
}

float4 toLinear(float4 gamma)
{
    return pow(gamma, 2.2f);
}

float fresnel(float3 I, float3 N)
{
    float  EtaValue = 1.0f / 1.33f;
    float  R0 = pow(1.0 - EtaValue, 2.0) / pow(1.0 + EtaValue, 2.0);

    float cosAngle = 1 - abs(dot(I, N));
    float result = cosAngle * cosAngle;
    result = result * result;
    result = result * cosAngle;
    result = min(.7, saturate(result * (1 - saturate(R0)) + R0));
    return result;
}

float radicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10f; // / 0x100000000
}

float2 Hammersley(uint i, uint N)
{
    return float2(float(i) / float(N), radicalInverse_VdC(i));
}

float2 Hammersley2(uint i, uint N)
{
    float ri = reversebits(i) * 2.3283064365386963e-10f;
    return float2(float(i) / float(N), ri);
}

float3 ImportanceSampleGGX(float2 Xi, float Roughness, float3 N)
{
    float a = Roughness * Roughness;
    float Phi = 2 * TS_PI * Xi.x;
    float CosTheta = sqrt((1 - Xi.y) / (1 + (a*a - 1) * Xi.y));
    float SinTheta = sqrt(1 - CosTheta * CosTheta);
    float3 H;
    H.x = SinTheta * cos(Phi);
    H.y = SinTheta * sin(Phi);
    H.z = CosTheta;
    float3 UpVector = abs(N.z) < 0.999 ? float3(0, 0, 1) : float3(1, 0, 0);
    float3 TangentX = normalize(cross(UpVector, N));
    float3 TangentY = cross(N, TangentX);
    // Tangent to world space
    return TangentX * H.x + TangentY * H.y + N * H.z;
}

float4 SampleIrradianceMap_SH(float3 WNormal)
{
    float4 outputColor = float4(0.0, 0.0, 0.0, 1.0);

        const float3 n = float3(WNormal.x, WNormal.y, -WNormal.z);
    // values from http://www.ppsloan.org/publications/StupidSH36.pdf#page=28
    const float4 sh_0_to_3 =
    {
        0.282095f,
        -0.488603f * n.y,
        0.488603f * n.z,
        -0.488603f * n.x
    };
    const float4 sh_4_to_7 =
    {
        1.092548f * n.x * n.y,
        -1.092548f * n.y * n.z,
        0.315392f * (3.0f * n.z * n.z - 1.0f),
        -1.092548f * n.x * n.z
    };
    const float sh_8 = 0.546274f * (n.x * n.x - n.y * n.y);

    outputColor.r = dot(sh_0_to_3, 1) + dot(sh_4_to_7, 1) + sh_8 * 1;
    outputColor.g = dot(sh_0_to_3, 1) + dot(sh_4_to_7, 1) + sh_8 * 1;
    outputColor.b = dot(sh_0_to_3, 1) + dot(sh_4_to_7, 1) + sh_8 * 1;
    outputColor.a = 1.0;

    return outputColor;
}


float3 PrefilterEnvMap( TextureCube EnvMap,
                        SamplerState EnvMapSampler,
                        float Roughness, float3 R)
{
    float TotalWeight;
    float3 N = R;
    float3 V = R;
    float3 PrefilteredColor = 0;
    const uint NumSamples = 1024;
    for (uint i = 0; i < NumSamples; i++)
    {
        float2 Xi = Hammersley2(i, NumSamples);
        float3 H = ImportanceSampleGGX(Xi, Roughness, N);
        float3 L = 2 * dot(V, H) * H - V;
        float NoL = saturate(dot(N, L));
        if (NoL > 0)
        {
            PrefilteredColor += EnvMap.SampleLevel(EnvMapSampler, L, 0).rgb * NoL;
            TotalWeight += NoL;
        }
    }
    return PrefilteredColor / TotalWeight;
}

float4 main( float4 pos		: SV_POSITION ,
             float2 uv : TEXCOORD0 ,
             float3 normal : TEXCOORD1 ,
             float4 worldPos : TEXCOORD2 ,
             Texture2D albedo : register(TEX_REGISTER_ALBEDO),
             TextureCube skyMap : register(t0 ),
             SamplerState samp : register( s0 ) ) : SV_TARGET
{
    float3 dir = float3( -1 , -1 , -0.5 );
    dir = normalize( dir );

    //albedo 
    float4 color = albedo.Sample( samp , uv ).rgba;

        //rambert
        float d = max( dot( normal , -dir ) , 0.1f );
    d = saturate( d );
    d = d* 0.5f + 0.5f;
    //compute CameraVector
    float3 eye = normalize( g_worldCameraPos.xyz - worldPos.xyz );

    //compute View And Camera HerfVector
    float3 VL = normalize( dir + eye );

    //fong
    float s = pow( max( saturate( dot( normal , VL ) ) , 0 ) , 80.0f );

    float3 refVec = reflect(eye, normal);

    //float4 res = skyMap.Sample( samp , refVec );

    // term1
    float2 size;
    float  numMipMapLevel;
    skyMap.GetDimensions(0, size.x, size.y, numMipMapLevel);

    float mipmapIndex = (g_fov)* (numMipMapLevel - 1.0f);
    float3 reflectDir = reflect(eye, normal);
    reflectDir.z *= -1.0f;

    float4 res = skyMap.SampleLevel(samp, reflectDir, mipmapIndex);

    //float3 res = PrefilterEnvMap(skyMap, samp, g_fov,refVec );
    //rimlight
    float rim = pow( 1 - saturate( dot( eye , normal ) ) , 12.5f );

    float4 result = ( d * color + s + rim );
        result = saturate( result );

    float f = fresnel(eye, normal);    

    float4 diff = SampleIrradianceMap_SH(normal) * (1-g_far);
    return diff + res * color ;
}