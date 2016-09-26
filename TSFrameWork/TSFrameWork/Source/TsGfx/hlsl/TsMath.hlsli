
static const float TS_PI = 3.14159265358979323846f;

float3 PackUnsigned( float3 data )
{
	return data * 0.5f + 0.5f;
}

float3 UnPackUnsigned( float3 data )
{
	return ( data - 0.5f ) * 2;
}

//! 2次補完
float ease_in( float t )
{
	return t*t;
}

//! 2次補完
float ease_out( float t )
{
	return t*(2-t);
}

//! 3次補完
float cubic( float t )
{
	return	t * t * ( 3.0f - 2.0f * t );
}

//! 5次補完
float HexLerp( float t )
{
	// t^3( t( 6*t - 15 ) + 10)
	return t * t * t * ( t * ( 6 * t - 15 ) + 10 );
}

//! 4点サンプリング
float4 Sample4( Texture2D tex , 
				SamplerState samp , 
				float2 texcoord ,
				float width,
				float height)
{
	float x = 1.0f / width;
	float y = 1.0f / height;
	float4 result = 0;
	result += tex.Sample( samp , texcoord );

	result += tex.Sample( samp , texcoord + float2( x , 0 ) );
	result += tex.Sample( samp , texcoord + float2(-x , 0 ) );
	result += tex.Sample( samp , texcoord + float2( 0 , y ) );
	result += tex.Sample( samp , texcoord + float2( 0 ,-y ) );

	return result / 5.0f;
}

//! 8点サンプリング
float4 Sample8( Texture2D tex ,
				SamplerState samp ,
				float2 texcoord ,
				float width ,
				float height )
{
	float x = 1.0f / width;
	float y = 1.0f / height;
	float4 result = 0;
	result += tex.Sample( samp , texcoord );

	result += tex.Sample( samp , texcoord + float2( x , 0 ) );
	result += tex.Sample( samp , texcoord + float2(-x , 0 ) );
	result += tex.Sample( samp , texcoord + float2( 0 , y ) );
	result += tex.Sample( samp , texcoord + float2( 0 ,-y ) );

	result += tex.Sample( samp , texcoord + float2( x , y ) );
	result += tex.Sample( samp , texcoord + float2(-x , y ) );
	result += tex.Sample( samp , texcoord + float2( x , y ) );
	result += tex.Sample( samp , texcoord + float2(-x ,-y ) );

	return result / 9.0f;
}

//1Dノイズ
float Noise1( float f )
{
	return (frac( HexLerp(frac( f ))*156.6363612*f*f*210.05f)-0.5f) * 2;
}

//2Dノイズ
float2 Noise2( float2 f )
{
	return float2( Noise1( f.x*14.01725f ) , 
				   Noise1( f.y*888.5051f ) );
}

//3Dノイズ
float3 Noise3( float3 f )
{
	return float3( Noise1( f.x*84.01759f ) , 
				   Noise1( f.y*888.5235f ) ,
				   Noise1( f.z*14.017215f ) );
}

//4Dノイズ
float4 Noise4( float4 f )
{
	return float4( Noise2( f.xw*84.876051f ),
				   Noise2( f.zy*64.022931f ) );
}
//
////1Dパーリンノイズ
//float PerlinNoise1( float f )
//{
//	return noise( f );
//}
//
////2Dパーリンノイズ
//float2 PerlinNoise2( float2 f )
//{
//	return noise( f );
//}
//
////3Dパーリンノイズ
//float3 PerlinNoise3( float3 f )
//{
//	return noise( f );
//}
//
////4Dパーリンノイズ
//float4 PerlinNoise4( float4 f )
//{
//	return noise( f );
//}