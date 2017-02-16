#include "TsParticleCommon.hlsli"

//---------------------------------------------------------------
//! GPUからRead & Write可能な領域
RWStructuredBuffer<ParticleEmitter> emitters : register(t6);
RWStructuredBuffer<Particle> particles : register(u0);

//---------------------------------------------------------------
// Particleを更新させる
//---------------------------------------------------------------
void Update(Particle particle)
{
    particle.life -= 1;
    particle.color  += particle.addtionalcolor;
    particle.radius += particle.addtionalradius;
    particle.position += particle.velocity;
}

//---------------------------------------------------------------
// エントリポイント
//---------------------------------------------------------------
[numthreads(1, 1, 1)]
void main(uint3 threadId : SV_DispatchThreadID)
{
    uint index = threadId.x;
    Update(particles[index]);
}