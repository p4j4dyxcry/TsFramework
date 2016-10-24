struct PS_SS_INPUT_UVx1
{
    float4 pos : SV_POSITION;
    float2 uv0 : TEXCOORD0;
};

struct PS_SS_INPUT_UVx2
{
    float4 pos : SV_POSITION;
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
};

struct PS_SS_INPUT_UVx3
{
    float4 pos : SV_POSITION;
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
    float2 uv2 : TEXCOORD2;
};

struct PS_SS_INPUT_UVx4
{
    float4 pos : SV_POSITION;
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
    float2 uv2 : TEXCOORD2;
    float2 uv3 : TEXCOORD3;
};

struct PS_SS_INPUT_UVx5
{
    float4 pos : SV_POSITION;
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
    float2 uv2 : TEXCOORD2;
    float2 uv3 : TEXCOORD3;
    float2 uv4 : TEXCOORD4;
};

struct PS_SS_INPUT_UVx6
{
    float4 pos : SV_POSITION;
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
    float2 uv2 : TEXCOORD2;
    float2 uv3 : TEXCOORD3;
    float2 uv5 : TEXCOORD5;
};

struct PS_SS_INPUT_UVx7
{
    float4 pos : SV_POSITION;
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
    float2 uv2 : TEXCOORD2;
    float2 uv3 : TEXCOORD3;
    float2 uv4 : TEXCOORD4;
    float2 uv5 : TEXCOORD5;
    float2 uv6 : TEXCOORD6;
};

struct PS_SS_INPUT_UVx8
{
    float4 pos : SV_POSITION;
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
    float2 uv2 : TEXCOORD2;
    float2 uv3 : TEXCOORD3;
    float2 uv4 : TEXCOORD4;
    float2 uv5 : TEXCOORD5;
    float2 uv6 : TEXCOORD6;
    float2 uv7 : TEXCOORD7;
};

float ConvertLinearDepth( float depth )
{
    float near = 1; // camera z near
    float far = 2000.0f; // camera z far
    float z = depth * 2.0 - 1.0; // Back to NDC 
    float dep = ( 2.0 * near ) / ( far + near - z * ( far - near ) );
    float v = ( far + near / ( far - near + dep ) ) * ( 2.0 / near );
    return ( v + 1.0 ) / 2; // Back to [0,1] range.
}