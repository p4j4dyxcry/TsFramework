/*
    hlsl 用の Utility 関数群
    
    一部関数をDefineマクロにしているのは、float4 float3 等の関数を
    オーバーライドで実装しなくてもよくなるため。

    © 2016 Yuki Tsuneyama
*/

// π
#define TS_PI (3.14159265358979323846f)

//! -1～1空間　を 0～1空間に変換
#define PackUnsigned( data ) \
    ( data * 0.5f + 0.5f )

//! 0～1空間　を -1～1空間に変換
#define UnPackUnsigned( data )  \
    ( ( data - 0.5f ) * 2 )

//! 2次補完
#define EaseIn( t ) \
    ( t*t )

//! 2次補完
#define EaseOut( t ) \
    ( t*(2-t) )

//! 3次補完
#define Cubic( t ) \
    ( t * t( 3.0f - 2.0f + t ) )

//! 5次補完
#define HexLerp \
    ( t * t * t * ( t * ( 6 * t - 15 ) + 10 ) )

//! テクスチャサイズの取得
float2 TexSize(Texture2D tex)
{
    float2 size;
    float  numMipMapLevel;
    tex.GetDimensions(0, size.x, size.y, numMipMapLevel);

    return size;
}

//! 4点サンプリング
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

//! 8点サンプリング
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