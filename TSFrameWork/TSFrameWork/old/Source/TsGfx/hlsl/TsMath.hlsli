
static const float TS_PI = 3.14159265358979323846f;

//! 2���⊮
float ease_in( float t )
{
	return t*t;
}

//! 2���⊮
float ease_out( float t )
{
	return t*(2-t);
}

//! 3���⊮
float cubic( float t )
{
	return	t * t * ( 3.0f - 2.0f * t );
}

//! 5���⊮
float HexLerp( float t )
{
	// t^3( t( 6*t - 15 ) + 10)
	return t * t * t * ( t * ( 6 * t - 15 ) + 10 );
}

//! 4�_�T���v�����O
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

//! 8�_�T���v�����O
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

//1D�m�C�Y
float Noize1( float f )
{
	return (frac( HexLerp(frac( f ))*156.6363612*f*f*210.05f)-0.5f) * 2;
}

//2D�m�C�Y
float2 Noize2( float2 f )
{
	return float2( Noize1( f.x*14.01725f ) , 
				   Noize1( f.y*888.5051f ) );
}

//3D�m�C�Y
float3 Noize3( float3 f )
{
	return float3( Noize1( f.x*84.01759f ) , 
				   Noize1( f.y*888.5235f ) ,
				   Noize1( f.z*14.017215f ) );
}

//4D�m�C�Y
float4 Noize4( float4 f )
{
	return float4( Noize2( f.xw*84.876051f ),
				   Noize2( f.zy*64.022931f ) , );
}

//1D�p�[�����m�C�Y
float PerlinNoise1( float f )
{
	return noize( f );
}

//2D�p�[�����m�C�Y
float2 PerlinNoise2( float2 f )
{
	return noize( f );
}

//3D�p�[�����m�C�Y
float3 PerlinNoise3( float3 f )
{
	return noize( f );
}

//4D�p�[�����m�C�Y
float4 PerlinNoise4( float4 f )
{
	return noize( f );
}