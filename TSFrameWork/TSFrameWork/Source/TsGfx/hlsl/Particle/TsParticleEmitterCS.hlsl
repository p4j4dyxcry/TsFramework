#include "TsParticleCommon.hlsli"

RWStructuredBuffer<ParticleEmitter> emitters    : register(t6);
RWStructuredBuffer<Particle> particles          : register(u0);

void Emit(ParticleEmitter emitter, Particle particle )
{
    particle.life = emitter.m_emitLife.start +
                    emitter.m_emitLife.randomRange *
                    ParticleRand(float3(492, 141, 41), emitter.m_emitRandomSeed);

    particle.life = emitter.m_emitLife.start +
                    emitter.m_emitLife.randomRange *
                    ParticleRand(float3(492, 141, 41), emitter.m_emitRandomSeed);

    //...���������E�E�E�Q�܂��B

}

[numthreads( 1 , 1 , 1 )]
void main( uint3 threadId : SV_DispatchThreadID )
{
    uint index = threadId.x;

    //todo : �o�ߎ��Ԃ͂Ƃ肠����60fps�z��ɂ��Ă���
    //       �K�v�������constantbuffer�Ȃ�ŊǗ�

    float elapsed = 0.016666666f;

    ParticleEmitter emitter = emitters[index];

    emitter.m_elasedTime += elapsed;

    //todo : rand�v�Z
    int emit_num = emitter.m_emitNum.start + 
                   emitter.m_emitNum.randomRange + 
                  ParticleRand(float3(3131, 5281, 4437), emitter.m_emitRandomSeed);

    if (emitter.m_elasedTime <= emitter.m_emitInterval)
    {
        //�G�~�b�^���������̃��[�v
        int emit_count = 0;
        for (int i = emitter.m_emitMemoryBegin; i < emitter.m_emitMemoryEnd; ++i)
        {
            Particle pt = particles[i];
            //todo �Ƃ肠�������x�v�Z�����Ă������邩���߂�B 
            //      if������̂��������S�T����������...
#if 0
            if (emit_count > emit_num)
                break;
#endif
            if( pt.life > 0 )
            {
                Emit(emitter, pt);
                //�������̃C���N�������g
                emit_count++;
            }            
        }
    }
}