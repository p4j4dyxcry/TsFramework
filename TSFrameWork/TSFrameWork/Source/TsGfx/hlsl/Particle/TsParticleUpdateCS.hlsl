#include "TsParticleCommon.hlsli"

//---------------------------------------------------------------
//! GPU����Read & Write�\�ȗ̈�
RWStructuredBuffer<ParticleEmitter> emitters : register(t6);
RWStructuredBuffer<Particle> particles : register(u0);

//---------------------------------------------------------------
// Particle���X�V������
//---------------------------------------------------------------
void Update(Particle particle)
{
    particle.life -= 1;
    particle.color  += particle.addtionalcolor;
    particle.radius += particle.addtionalradius;
    particle.position += particle.velocity;
}

//---------------------------------------------------------------
// �G���g���|�C���g
//---------------------------------------------------------------
[numthreads(1, 1, 1)]
void main(uint3 threadId : SV_DispatchThreadID)
{
    uint index = threadId.x;
    Update(particles[index]);
}