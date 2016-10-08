
cbuffer matrixs : register (b0)
{
    float4x4 g_MtxWorld;
    float4x4 g_MtxInvWorld;
}

cbuffer ViewCB : register (b7)
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

cbuffer light : register(b1)
{
    float4 g_lightDir = float4(0,1,0,0);
};

struct VS_DEFAULT_INPUT
{
    float3 pos      : SV_POSITION;
    float3 normal   : NORMAL;
    float2 uv       : TEXCOORD0;
};

struct VS_SKIN_INPT
{
    float3 pos      : SV_POSITION;
    float3 normal   : NORMAL;
    float2 uv       : TEXCOORD0;
    float4 weight   : WEIGHT;
    uint4  boneIdx  : TEXCOORD1;
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