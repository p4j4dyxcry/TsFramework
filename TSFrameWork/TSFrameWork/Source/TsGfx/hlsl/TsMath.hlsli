
static const float TS_PI = 3.14159265358979323846f;

//! -1�`1��ԁ@�� 0�`1��Ԃɕϊ�
#define PackUnsigned( data )    ( data * 0.5f + 0.5f )

//! 0�`1��ԁ@�� -1�`1��Ԃɕϊ�
#define UnPackUnsigned( data )  ( ( data - 0.5f ) * 2 )

//! 2���⊮
#define EaseIn( t ) ( t*t )

//! 2���⊮
#define EaseOut( t ) ( t*(2-t) )

//! 3���⊮
#define Cubic( t ) ( t * t( 3.0f - 2.0f + t ) )

//! 5���⊮
#define HexLerp ( t * t * t * ( t * ( 6 * t - 15 ) + 10 ) )

//! �e�N�X�`���T�C�Y�̎擾
float2 TexSize(Texture2D tex)
{
    float2 size;
    float  numMipMapLevel;
    tex.GetDimensions(0, size.x, size.y, numMipMapLevel);

    return size;
}

//! 4�_�T���v�����O
float4 Sample4( Texture2D tex , 
                SamplerState samp , 
                float2 texcoord )
{
    float2 size =  1.0f / TexSize(tex);
    float x = size.x;
    float y = size.y;
        
    float4 result = 0;

    result += tex.Sample( samp , texcoord + float2( x , 0 ) );
    result += tex.Sample( samp , texcoord + float2(-x , 0 ) );
    result += tex.Sample( samp , texcoord + float2( 0 , y ) );
    result += tex.Sample( samp , texcoord + float2( 0 ,-y ) );

    return result / 5.0f;
}

//! 8�_�T���v�����O
float4 Sample8( Texture2D tex ,
                SamplerState samp ,
                float2 texcoord )
{
    float2 size =  1.0f / TexSize(tex);
    float x = size.x;
    float y = size.y;
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