#include "TsParticleCommon.hlsli"

//---------------------------------------------------------------
//! GPUからRead & Write可能な領域
RWStructuredBuffer<ParticleEmitter> emitters    : register(t6);
RWStructuredBuffer<Particle> particles          : register(u0);

//---------------------------------------------------------------
// Particleを発生させる
//---------------------------------------------------------------
void Emit(ParticleEmitter emitter, Particle particle )
{
    particle.maxLife = 
    particle.life  = (uint)RandomInitialize(float3(512, 215, 52), emitter.m_emitLife, emitter.m_emitRandomSeed);

    particle.color = RandomInitialize(float3(17, 63, 96), emitter.m_emitColor, emitter.m_emitRandomSeed);
    particle.position = RandomInitialize(float3(17, 63, 96), emitter.m_position, emitter.m_emitRandomSeed);
    particle.radius = RandomInitialize(float3(131, 519, 532), emitter.m_emitRadius, emitter.m_emitRandomSeed);

    // todo
    particle.gravity = 0;
    particle.addtionalradius = ToAdditional(particle.radius, emitter.m_emitRadius, particle.life);
    particle.addtionalcolor  = ToAdditional(particle.color, emitter.m_emitColor, particle.life);

    particle.velocity = ToAdditional(emitter.m_emitVelocity.start, emitter.m_emitVelocity, particle.life);
}

[numthreads( 1 , 1 , 1 )]
void main( uint3 threadId : SV_DispatchThreadID )
{
    uint index = threadId.x;

    //todo : 経過時間はとりあえず60fps想定にしておく
    //       必要があればconstantbufferなりで管理

    float elapsed = 0.016666666f;

    ParticleEmitter emitter = emitters[index];

    emitter.m_elasedTime += elapsed;

    //todo : rand計算
    int emit_num = (int)RandomInitialize(float3(3131, 5281, 4437), emitter.m_emitNum, emitter.m_emitRandomSeed);

    if (emitter.m_elasedTime <= emitter.m_emitInterval)
    {
        //エミッタメモリ分のループ
        int emit_count = 0;
        for (int i = emitter.m_emitMemoryBegin; i < emitter.m_emitMemoryEnd; ++i)
        {
            Particle pt = particles[i];
            //todo とりあえず速度計算をしてから入れるか決める。 
            //      ifを入れるのが早いか全探査が早いか...
#if 0
            if (emit_count > emit_num)
                break;
#endif
            if( pt.life > 0 )
            {
                Emit(emitter, pt);
                //生成数のインクリメント
                emit_count++;
            }            
        }
    }
}