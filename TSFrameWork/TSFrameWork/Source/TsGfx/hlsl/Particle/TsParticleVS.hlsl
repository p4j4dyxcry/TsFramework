#include "../TsDefaultShading_Common.hlsli"
#include "TsParticleCommon.hlsli"

StructuredBuffer<Particle> g_particles : register(u0);

PARTICLE_VS_OUT main(PARTICLE_VS_IN vsIn )
{
    PARTICLE_VS_OUT vsout = (PARTICLE_VS_OUT)(0);

    uint idx = vsIn.InstanceID;
    Particle pt = g_particles[idx];
    vsout.position = pt.position;
    vsout.color = pt.color;
    vsout.size = pt.radius;

    //life 0‚Ì‚Æ‚«‚ÍƒNƒŠƒbƒv‚·‚é
    vsout.size *= pt.life > 0 ? 1 : 0;

    return vsout;
}