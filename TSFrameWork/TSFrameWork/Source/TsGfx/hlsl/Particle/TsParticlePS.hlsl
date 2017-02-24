#include "../TsDefaultShading_Common.hlsli"
#include "TsParticleCommon.hlsli"

PARTICLE_PS_OUT main( PARTICLE_PS_IN _in )
{
    PARTICLE_PS_OUT ps_out;
    ps_out.color = _in.color;
    return ps_out;
}