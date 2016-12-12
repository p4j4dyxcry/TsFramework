
#define TEX_REGISTER_ALBEDO     t8
#define TEX_REGISTER_NORMAL     t9
#define TEX_REGISTER_SPECULER   t10


#define CBUFFER_OBJ_MTX_REGISTER  b0
#define CBUFFER_SKIN_REGISTER b1
#define CBUFFER_LIGHT_SET_REGISTER b2
#define CBUFFER_MATERIAL_REGISTER b3
#define CBUFFER_INSTANCE_REGISTER b4
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

cbuffer matrixs : register ( CBUFFER_INSTANCE_REGISTER )
{
    float4x4 g_MtxInstance[1024];
}

cbuffer MaterialCB : register ( CBUFFER_MATERIAL_REGISTER )
{
    float4 g_Diffuse;       // xyz = diffuse w = alphaTest
    float4 g_Specluar;      // xyz = color w = power
    float4 g_Ambient;       // xyz = color w = not using
    float3 g_Emissive;      // xyz = emissive w
    float  g_Mataric;       // mataric
    float  g_Roughness;     // roughness

    float  g_useDiffuseMap; // true on using albedo map
    float  g_useNomalMap;   // true on using nomal map
    float  g_useSpeculerMap;// true on using speclur map
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
    float ViewCB_Dummy;
}

struct LightData
{
    float4x4         worldToShadowMatrix;    //directional Only

    float4           color;
    float4           pos;
    float4           dir;   // directional onry

    uint             type;  // 0 directional / 1 point / 2 spot
    float            intensity;
    float            range; // spot & point 
    float            angle; // spot only

};

cbuffer LightSetCB : register(CBUFFER_LIGHT_SET_REGISTER)
{
    LightData           g_LightData[256];
    uint                g_LightNum;
    float3              LightSetCBufferDummy;
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
    uint   instanceID: SV_InstanceID;
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
    float4 pos = float4( v.pos , 1 );

    if( v.weight[0] + v.weight[1] + v.weight[2] + v.weight[3] > 0.05f )
    {  
        float4 bpos0 = v.weight[0] * mul( pos , g_BoneMatrix[v.boneIdx.x] );
        float4 bpos1 = v.weight[1] * mul( pos , g_BoneMatrix[v.boneIdx.y] );
        float4 bpos2 = v.weight[2] * mul( pos , g_BoneMatrix[v.boneIdx.z] );
        float4 bpos3 = v.weight[3] * mul( pos , g_BoneMatrix[v.boneIdx.w] );

        pos = bpos0 + bpos1 + bpos2 + bpos3;
    }

    return pos;
}

float3 ComputeSkinNormal( VS_SKIN_INPUT v )
{
    float3 pos = float3( v.normal );

    if( v.weight[0] + v.weight[1] + v.weight[2] + v.weight[3] > 0.05f )
    { 
        float3 bpos0 = v.weight[0] * mul( pos , ( float3x3 )g_BoneMatrix[v.boneIdx.x] );
        float3 bpos1 = v.weight[1] * mul( pos , ( float3x3 )g_BoneMatrix[v.boneIdx.y] );
        float3 bpos2 = v.weight[2] * mul( pos , ( float3x3 )g_BoneMatrix[v.boneIdx.z] );
        float3 bpos3 = v.weight[3] * mul( pos , ( float3x3 )g_BoneMatrix[v.boneIdx.w] );

        pos = bpos0 + bpos1 + bpos2 + bpos3;
    }

    return pos;
}

float4 DepthToWorldPos(float2 vTexCoord , float depth)
{
    // Get x/w and y/w from the viewport position
    float x = vTexCoord.x * 2 - 1;
    float y = (1 - vTexCoord.y) * 2 - 1;
    float z = depth;

    float4 projPos = float4(x, y, z, 1.0f);
    
    float4 viewPos = mul(projPos, g_MtxInvProj);

    float4 worldPos = mul(viewPos, g_MtxInvView);
    worldPos.xyz /= worldPos.w;

    return worldPos;
}

float DepthToLinear(float depth)
{
    // rhs
    //x = proj[2][2], 
    //y = -proj[3][2], 
    //z = proj[2][3], 
    //w = -proj[3][3]

    float4x4 proj = g_MtxProj;

    float x = proj[2][2];
    float y = -proj[3][2];
    float z = proj[2][3];
    float w = -proj[3][3];

    float linearDepth = 
          mad(depth, w, -y) / mad(-depth, z, x);

    return linearDepth;
}

float2 UVToScreenPos(float2 uv)
{
    return mad(uv, float2(2.0f, -2.0f), float2(-1.0f, 1.0f));
}