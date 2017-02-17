#include "../TsDefaultShading_Common.hlsli"
#include "TsParticleCommon.hlsli"

PARTICLE_PS_OUT main( PARTICLE_PS_IN _in )
{
    return _in.color;
}