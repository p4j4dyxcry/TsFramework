#include "../TsDefaultShading_Common.hlsli"
#include "TsParticleCommon.hlsli"

[maxvertexcount(3)]
void main( point PARTICLE_GS_IN _in[1],
           inout TriangleStream<PARTICLE_GS_OUT> _out)
{
    const float3 vertex_pos[4] =
    {
        -0.5f, 0.5f , 0,
         0.5f, 0.5f , 0,
        -0.5f, -0.5f, 0,
         0.5f, -0.5f, 0,
    };
    const float2 vertex_uv[4] =
    {
        0, 0,
        1, 0,
        0, 1,
        1, 1
    };

    int i = 0;

    for (i = 0; i < 4; ++i)
    {
        PARTICLE_GS_OUT gs_out;
        gs_out.color = _in[0].color;
        gs_out.uv = vertex_uv[i];
        gs_out.position = _in[0].position;
        gs_out.position.xyz += vertex_pos[i] * _in[0].size;

        //todo ビルボード
        gs_out.position = mul(gs_out.position, g_MtxVP);

        _out.Append(gs_out);      
    }
}