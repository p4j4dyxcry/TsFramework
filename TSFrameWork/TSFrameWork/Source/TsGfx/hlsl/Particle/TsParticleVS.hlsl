#include "../TsDefaultShading_Common.hlsli"
#include "TsParticleCommon.hlsli"

StructuredBuffer<Particle> g_particles : register(u0);

PARTICLE_VS_OUT main(PARTICLE_VS_IN vsIn )
{
    PARTICLE_VS_OUT vsout = (PARTICLE_VS_OUT)(0);

    uint idx = vsIn.InstanceID;
    Particle pt = g_particles[idx];
    vsout.position = float4( pt.position,1);
    vsout.color = pt.color;
    vsout.size = pt.radius;

    //life 0のときはクリップする
    vsout.size *= pt.life > 0 ? 1 : 0;

    return vsout;
}