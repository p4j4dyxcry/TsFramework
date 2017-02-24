//-------------------------------------------------------------------------
//
// パーティクルの処理フローについて
// ・CPUからエミッタ情報を入力
// ■ ComputeShaderでパーティクルのエミット
//    ->structuredBufferにエミッタ情報を格納
//    ->RWStructuredBufferに粒子情報を出力
// ■ComputeShaderでパーティクルのアップデート
//    ->RWStructuredBufferから粒子情報を入力
//    ->RWStructuredBufferに粒子情報を出力
// ■VertexShaderでRWstructuredBufferをstructuredBuffer
//  としてGeometryShaderにデータを流す
// ■GeometoryShaderで入力された頂点を4つに変換しView＆Projection変換を行う
// ■PielShaderで出力
//-------------------------------------------------------------------------

//! float1のエミットパラメータ
struct float1EmitterParam
{
    float start;        //開始
    float randomRange;  //振れ幅
    float end;          //終了

    // バイトアライメント調整
    float dumy;
};

//! float2のエミットパラメータ
struct float2EmitterParam
{
    float2 start;       //開始
    float2 randomRange; //振れ幅
    float2 end;         //終了

    // バイトアライメント調整
    float2 dumy;
};

//! float3のエミットパラメータ
struct float3EmitterParam
{
    float3 start;       //開始
    float3 randomRange; //振れ幅
    float3 end;         //終了

    // バイトアライメント調整
    float3 dumy;
};

//! float4のエミットパラメータ
struct float4EmitterParam
{
    float4 start;       //開始
    float4 randomRange; //振れ幅
    float4 end;         //終了

    float4 dumy;
};

//---------------------------------------------------------------
//  *ReadMe
//  パーティクルエミッタではEmitterParamの情報を入力することでアニメート
//  をサポートします。なお、アニメートを行なわないパラメータはEmitterParam
//  のstartとendを同値にすることでアニメートを無効にできます。
//  アニメートの計算式は下記であらわされます。
//
//  【t】= 正規化残りライフ 【凡例】開始life が10で残りlifeが3 のときは0.3f
//  【s】= EmitterParam.start + RandomValue
//　 strat*t + end(1-t)
//---------------------------------------------------------------
//! パーティクルエミッター
//---------------------------------------------------------------
struct ParticleEmitter
{
    //! パーティクルのエミッタの位置
    float3EmitterParam m_position;

    //! パーティクルのエミッタ半径
    float1EmitterParam m_emitRadius;

    //! パーティクルのエミッタ色
    float4EmitterParam m_emitColor;

    //! パーティクルのエミッタの初速
    float3EmitterParam m_emitVelocity;

    //! エミット数
    float1EmitterParam m_emitNum;

    //! パーティクルライフ
    float1EmitterParam m_emitLife;

    //! 使用するメモリの開始インデックス
    uint m_emitMemoryBegin;

    //! 同上、終了インデックス
    uint m_emitMemoryEnd;

    //! 発射間隔(sec)
    float m_emitInterval;

    //! 経過時間
    float m_elasedTime;

    //! エミッタの乱数の種
    float m_emitRandomSeed;

    // バイトアライメント調整
    float3 dumy;
};

//---------------------------------------------------------------
//! パーティクル構造体
//---------------------------------------------------------------
struct Particle
{
    //! 位置を決定する要素
    float3  position;
    float3  velocity;
    float   gravity;

    //! 大きさを決定する要素
    float   radius;
    float   addtionalradius;

    //! 寿命を決定する要素
    float   life;
    float   maxLife;

    //! 色を決定する要素
    float4  color;
    float4  addtionalcolor;

    // バイトアライメント調整
    float   dumy;
};

//---------------------------------------------------------------
//! 乱数を変更する際にはこの関数を書き換えます。
//---------------------------------------------------------------
float ParticleRand(float3 value ,float seed)
{
    return frac(cos(dot(value ,float3(428.1529f,582.952f,94.00425f)) + seed) * 5343.3414f);
}



//---------------------------------------------------------------
//! 重力計算を変更する際にはこの関数を書き換えます。
//---------------------------------------------------------------
float ComputeGravity(Particle particle)
{
    //todo : test
    return particle.gravity * 1.08f;
    //return t * t * (3.0f - 2.0f + t);
}

//---------------------------------------------------------------
//! 補完式を変更する際にはこの関数を書き換えます。
//---------------------------------------------------------------
float ComputeTFunc( float t )
{
    //todo : test
    return t;
    //return t * t * (3.0f - 2.0f + t);
}

//---------------------------------------------------------------
//! 1フレームでの変化量を計算します。
//---------------------------------------------------------------
float ToAdditional(float start, float1EmitterParam value, float life)
{
    return lerp(start, value.end, ComputeTFunc(1.0f / life));
}

float2 ToAdditional(float2 start, float2EmitterParam value, float life)
{
    return lerp(start, value.end, ComputeTFunc(1.0f / life));
}

float3 ToAdditional(float3 start, float3EmitterParam value, float life)
{
    return lerp(start, value.end, ComputeTFunc(1.0f / life));
}

float4 ToAdditional(float4 start , float4EmitterParam value, float life)
{
    return lerp(start, value.end, ComputeTFunc(1.0f / life));
}

float RandomInitialize(float3 value, float1EmitterParam emitterParam, float seed)
{
    return emitterParam.start +
           emitterParam.randomRange *
           ParticleRand(value, seed);
}

float2 RandomInitialize(float3 value, float2EmitterParam emitterParam, float seed)
{
    return emitterParam.start +
           emitterParam.randomRange *
           ParticleRand(value, seed);
}

float3 RandomInitialize(float3 value, float3EmitterParam emitterParam, float seed)
{
    return emitterParam.start +
           emitterParam.randomRange *
           ParticleRand(value, seed);
}

float4 RandomInitialize(float3 value, float4EmitterParam emitterParam, float seed)
{
    return emitterParam.start +
           emitterParam.randomRange *
           ParticleRand(value, seed);
}

//---------------------------------------------------------------
//! 頂点シェーダの入力
//---------------------------------------------------------------
struct PARTICLE_VS_IN
{
    uint InstanceID : SV_InstanceID;
};

//---------------------------------------------------------------
//! 頂点シェーダの出力 & ジオメトリシェーダの入力
//---------------------------------------------------------------
struct PARTICLE_VS_OUT
{
    float4 position : SV_Position;
    float4 color    : COLOR0;
    float  size     : TEXCOORD0;
};
#define PARTICLE_GS_IN PARTICLE_VS_OUT

//---------------------------------------------------------------
//! ジオメトリシェーダの出力 & ピクセルシェーダの入力 
//---------------------------------------------------------------
struct PARTICLE_GS_OUT
{
    float4 position : SV_Position;
    float4 color    : COLOR0;
    float2 uv       : TEXCOORD0;
};
#define PARTICLE_PS_IN PARTICLE_GS_OUT

//---------------------------------------------------------------
//! ピクセルシェーダの出力
//---------------------------------------------------------------
struct PARTICLE_PS_OUT
{
    float4 color : SV_Target0;
};
