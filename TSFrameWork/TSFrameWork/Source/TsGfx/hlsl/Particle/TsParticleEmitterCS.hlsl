#include "TsParticleCommon.hlsli"

//---------------------------------------------------------------
//! GPU����Read & Write�\�ȗ̈�
RWStructuredBuffer<ParticleEmitter> emitters    : register(t6);
RWStructuredBuffer<Particle> particles          : register(u0);

//---------------------------------------------------------------
// Particle�𔭐�������
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

    //todo : �o�ߎ��Ԃ͂Ƃ肠����60fps�z��ɂ��Ă���
    //       �K�v�������constantbuffer�Ȃ�ŊǗ�

    float elapsed = 0.016666666f;

    ParticleEmitter emitter = emitters[index];

    emitter.m_elasedTime += elapsed;

    //todo : rand�v�Z
    int emit_num = (int)RandomInitialize(float3(3131, 5281, 4437), emitter.m_emitNum, emitter.m_emitRandomSeed);

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