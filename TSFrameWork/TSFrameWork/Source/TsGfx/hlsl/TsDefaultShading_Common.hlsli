
#define TEX_REGISTER_ALBEDO t8


#define CBUFFER_OBJ_MTX_REGISTER  b0
#define CBUFFER_SKIN_REGISTER b1
#define CBUFFER_LIGHT_SET_REGISTER b2
#define CBUFFER_MATERIAL_REGISTER b3
#define CBUFFER_VIEW_REGISTER b7


cbuffer matrixs : register (CBUFFER_OBJ_MTX_REGISTER)
{
    float4x4 g_MtxWorld;
    float4x4 g_MtxInvWorld;
}

cbuffer BoneMatrixs : register (CBUFFER_SKIN_REGISTER)
{
    float4x4 g_BoneMatrix[512];
}

cbuffer ViewCB : register (CBUFFER_VIEW_REGISTER)
{
    float4x4 g_MtxView;
    float4x4 g_MtxProj;
    float4x4 g_MtxInvView;
    float4x4 g_MtxInvProj;
    float4x4 g_MtxVP;

    float4 g_worldCameraPos;

    float g_near;
    float g_far;
    float g_fov;
    float ViewCB_Dumy;
}

struct LightData
{
    uint             type;  // 0 directional / 1 point / 2 spot
    float4           color;
    float3           pos;
    float3           dir;   // directional onry
    float            intensity;
    float            range; // spot & point 
    float            angle; // spot only
    float4x4         worldToShadowMatrix;    //directional Only
};

cbuffer LightSetCB : register(CBUFFER_LIGHT_SET_REGISTER)
{
    LightData           g_LightData[256];
    uint                g_LightNum;
};

struct VS_DEFAULT_INPUT
{
    float3 pos      : SV_POSITION;
    float3 normal   : NORMAL;
    float2 uv       : TEXCOORD0;
};

struct VS_SKIN_INPUT
{
    float3 pos      : SV_POSITION;
    float3 normal   : NORMAL;
    float4 weight   : WEIGHT;
    uint4  boneIdx  : TEXCOORD1;
    float2 uv       : TEXCOORD0;
};

// Foward / Defferd / ScreenSpace
struct PS_OUTPUT
{
    float4 data0 : SV_TARGET0;
};

// 出力先が2つのGBuffer
struct PS_GBUFFER_OUTPUT_2
{
    //===============================
    // data 0
    // MainColor     = r g b 
    // スペキュラ	     = a
    float4 data0: SV_TARGET0;

    //===============================
    // data 1
    // PixelNormal	= r g b 
    // Depth        = a
    float4 data1: SV_TARGET1;
};

// 出力先が3つのGBuffer
struct PS_GBUFFER_OUTPUT_3
{
    //===============================
    // data 0
    // MainColor     = r g b 
    // スペキュラ	     = a
    float4 data0: SV_TARGET0;

    //===============================
    // data 1
    // PixelNormal  = r g b 
    // Depth        = a
    float4 data1: SV_TARGET1;

    //===============================
    // data 2
    // ラフネス     = r
    // メタリック    = g
    float4 data2: SV_TARGET2;
};

// 出力先が4つのGBuffer
struct PS_GBUFFER_OUTPUT_4
{
    //===============================
    // data 0
    // MainColor    = r g b 
    // スペキュラ     = a
    float4 data0: SV_TARGET0; 

    //===============================
    // data 1
    // PixelNormal  = r g b 
    // Depth        = a
    float4 data1: SV_TARGET1; 

    //===============================
    // data 2
    // ラフネス      s = r
    // メタリック	    = g
    float4 data2: SV_TARGET2;

    //===============================
    // data 3
    // World Pos = r g b
    float4 data3: SV_TARGET3;
};

float4 ComputeSkinMesh( VS_SKIN_INPUT v )
{
    if( v.weight[0] + v.weight[1] + v.weight[2] + v.weight[3] < 0.05f )
        return float4( v.pos , 1 );

    float4 pos = float4( v.pos , 1 );

        float4 bpos0 = v.weight[0] * mul( pos , g_BoneMatrix[v.boneIdx.x] );
        float4 bpos1 = v.weight[1] * mul( pos , g_BoneMatrix[v.boneIdx.y] );
        float4 bpos2 = v.weight[2] * mul( pos , g_BoneMatrix[v.boneIdx.z] );
        float4 bpos3 = v.weight[3] * mul( pos , g_BoneMatrix[v.boneIdx.w] );

        pos = bpos0 + bpos1 + bpos2 + bpos3;

    return pos;
}

float3 ComputeSkinNormal( VS_SKIN_INPUT v )
{
    if( v.weight[0] + v.weight[1] + v.weight[2] + v.weight[3] < 0.05f )
        return float3( v.normal  );

    float3 pos = float3( v.normal );

        float3 bpos0 = v.weight[0] * mul( pos , ( float3x3 )g_BoneMatrix[v.boneIdx.x] );
        float3 bpos1 = v.weight[1] * mul( pos , ( float3x3 )g_BoneMatrix[v.boneIdx.y] );
        float3 bpos2 = v.weight[2] * mul( pos , ( float3x3 )g_BoneMatrix[v.boneIdx.z] );
        float3 bpos3 = v.weight[3] * mul( pos , ( float3x3 )g_BoneMatrix[v.boneIdx.w] );

        pos = bpos0 + bpos1 + bpos2 + bpos3;

    return pos;
}

float3 DepthToWorldPos(float2 vTexCoord , float depth)
{
    // Get x/w and y/w from the viewport position
    float x = vTexCoord.x * 2 - 1;
    float y = (1 - vTexCoord.y) * 2 - 1;
    float z = depth;

    float4 vProjectedPos = float4(x, y, z, 1.0f);
    // Transform by the inverse projection matrix
    float4 vPositionVS = mul(vProjectedPos, g_MtxInvProj);
    // Divide by w to get the view-space position
    return vPositionVS.xyz / vPositionVS.w;
}
