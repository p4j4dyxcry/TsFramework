
cbuffer matrixs : register (b0)
{
	float4x4 g_MtxWorld;
	float4x4 g_MtxInvWorld;

	float4x4 g_MtxView;
	float4x4 g_MtxInvView;

	float4x4 g_MtxProj;
	float4x4 g_MtxInvProj;

	float4x4 g_MtxVP;
	float4x4 g_MtxInvVP;
}

struct VS_DEFAULT_INPUT
{
	float3 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float3 normal	: NORMAL;
};

struct VS_SKIN_INPT
{
	float3 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float3 normal	: NORMAL;
	float4 weight	: WEIGHT;
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
	// MainColor	 = r g b 
	// スペキュラ	 = a
	float4 data0: SV_TARGET0;

	//===============================
	// data 1
	// PixelNormal	= r g b 
	// Depth		= a
	float4 data1: SV_TARGET1;
};

// 出力先が3つのGBuffer
struct PS_GBUFFER_OUTPUT_3
{
	//===============================
	// data 0
	// MainColor	 = r g b 
	// スペキュラ	 = a
	float4 data0: SV_TARGET0;

	//===============================
	// data 1
	// PixelNormal	= r g b 
	// Depth		= a
	float4 data1: SV_TARGET1;

	//===============================
	// data 2
	// ラフネス		= r
	// メタリック	= g
	float4 data2: SV_TARGET2;
};

// 出力先が4つのGBuffer
struct PS_GBUFFER_OUTPUT_4
{
	//===============================
	// data 0
	// MainColor	 = r g b 
	// スペキュラ	 = a
	float4 data0: SV_TARGET0; 

	//===============================
	// data 1
	// PixelNormal	= r g b 
	// Depth		= a
	float4 data1: SV_TARGET1; 

	//===============================
	// data 2
	// ラフネス		= r
	// メタリック	= g
	float4 data2: SV_TARGET2;

	//===============================
	// data 3
	// World Pos = r g b
	float4 data3: SV_TARGET3;
};