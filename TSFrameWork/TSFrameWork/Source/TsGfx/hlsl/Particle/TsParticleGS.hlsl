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

    float4x4 mtxBillboard;
    mtxBillboard[0][0] = g_MtxView[0][0];
    mtxBillboard[0][1] = g_MtxView[1][0];
    mtxBillboard[0][2] = g_MtxView[2][0];
    mtxBillboard[0][3] = 0;

    mtxBillboard[1][0] = g_MtxView[0][1];
    mtxBillboard[1][1] = g_MtxView[1][1];
    mtxBillboard[1][2] = g_MtxView[2][1];
    mtxBillboard[1][3] = 0;

    mtxBillboard[2][0] = g_MtxView[0][2];
    mtxBillboard[2][1] = g_MtxView[1][2];
    mtxBillboard[2][2] = g_MtxView[2][2];
    mtxBillboard[2][3] = 0;

    mtxBillboard[3][0] = g_MtxView[0][3];
    mtxBillboard[3][1] = g_MtxView[1][3];
    mtxBillboard[3][2] = g_MtxView[2][3];
    mtxBillboard[3][3] = 1.0f;

    for (i = 0; i < 4; ++i)
    {
        PARTICLE_GS_OUT gs_out;
        gs_out.color = _in[0].color;
        gs_out.uv = vertex_uv[i];
        gs_out.position = _in[0].position;
        gs_out.position.xyz += vertex_pos[i] * _in[0].size;

        gs_out.position = mul(gs_out.position, mtxBillboard);
        gs_out.position = mul(gs_out.position, g_MtxVP);

        _out.Append(gs_out);      
    }
}