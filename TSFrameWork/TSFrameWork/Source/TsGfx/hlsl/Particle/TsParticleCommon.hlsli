
//! float1のエミットパラメータ
struct float1EmitterParam
{
    float start;        //開始
    float randomRange;  //振れ幅
    float end;          //終了

    float dumy;
};

//! float2のエミットパラメータ
struct float2EmitterParam
{
    float2 start;       //開始
    float2 randomRange; //振れ幅
    float2 end;         //終了

    float2 dumy;
};

//! float3のエミットパラメータ
struct float3EmitterParam
{
    float3 start;       //開始
    float3 randomRange; //振れ幅
    float3 end;         //終了

    float3 dumy;
};

//! float4のエミットパラメータ
struct float4EmitterParam
{
    float4 start;       //開始
    float4 randomRange; //振れ幅
    float4 end;         //終了
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

    float3 dumy;
};

//---------------------------------------------------------------
//! パーティクル構造体
//---------------------------------------------------------------
struct Particle
{
    float3  position;
    float   radius;
    float3  velocity;
    float   life;
    float   maxLife;
    float3  gravity;
    float4  color;
    float4  addtionalcolor;
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
float3 ComputeGravity(Particle particle)
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

float ToAdditional(float1EmitterParam value , float life)
{
    return lerp(value.start, value.end, ComputeTFunc(1.0f / life));
}

float2 ToAdditional(float2EmitterParam value, float life)
{
    return lerp(value.start, value.end, ComputeTFunc(1.0f / life));
}

float3 ToAdditional(float3EmitterParam value, float life)
{
    return lerp(value.start, value.end, ComputeTFunc(1.0f / life));
}

float4 ToAdditional(float4EmitterParam value, float life)
{
    return lerp(value.start, value.end, ComputeTFunc(1.0f / life));
}



