#include "../../../TsAfx.h"

TsSkyBox::TsSkyBox() :
m_pVertexBuffer(nullptr)
{

}

TsSkyBox::~TsSkyBox()
{
    TsSafeDelete(m_pVertexBuffer);
    TsSafeDelete(m_pIndexBuffer);

    TsSafeDelete(m_pTransform);
    TsSafeDelete(m_pTransformCBuffer);
}
TsBool TsSkyBox::Create(TsDevice* pDev)
{
    // front
    m_vertex[0].m_pos = TsVector4(-1,  1, 1, 1);
    m_vertex[1].m_pos = TsVector4( 1,  1, 1, 1);
    m_vertex[2].m_pos = TsVector4(-1, -1, 1, 1);
    m_vertex[3].m_pos = TsVector4( 1, -1, 1, 1);

    // back
    m_vertex[4].m_pos = TsVector4(-1,  1, -1, 1);
    m_vertex[5].m_pos = TsVector4( 1,  1, -1, 1);
    m_vertex[6].m_pos = TsVector4(-1, -1, -1, 1);
    m_vertex[7].m_pos = TsVector4( 1, -1, -1, 1);

    // up
    m_vertex[8].m_pos = TsVector4(- 1, 1, -1, 1);
    m_vertex[9].m_pos = TsVector4(  1, 1, -1, 1);
    m_vertex[10].m_pos = TsVector4(-1, 1,  1, 1);
    m_vertex[11].m_pos = TsVector4( 1, 1,  1, 1);

    // down
    m_vertex[12].m_pos = TsVector4(-1, -1, -1, 1);
    m_vertex[13].m_pos = TsVector4( 1, -1, -1, 1);
    m_vertex[14].m_pos = TsVector4(-1, -1,  1, 1);
    m_vertex[15].m_pos = TsVector4( 1, -1,  1, 1);

    // left
    m_vertex[16].m_pos = TsVector4(-1,  1, -1, 1);
    m_vertex[17].m_pos = TsVector4(-1,  1,  1, 1);
    m_vertex[18].m_pos = TsVector4(-1, -1,  1, 1);
    m_vertex[19].m_pos = TsVector4(-1, -1, -1, 1);

    // right
    m_vertex[20].m_pos = TsVector4(1, -1, -1, 1);
    m_vertex[21].m_pos = TsVector4(1,  1, -1, 1);
    m_vertex[22].m_pos = TsVector4(1, -1,  1, 1);
    m_vertex[23].m_pos = TsVector4(1,  1,  1, 1);

    for (int i = 0; i < 24; ++i)
        m_vertex[i].m_pos *= 100;

    m_pVertexBuffer = pDev->CreateVertexBuffer(
        m_vertex, 
        sizeof(m_vertex), 
        sizeof(TSVertexSkyBox), 0);

    // Index
    TsInt index[36] =
    {
        //front
        0, 2, 1,
        2, 3, 1,

        //back
        6, 5, 4,
        7, 6, 5,

        //up
        10,  9,  8,
        11, 10,  9,

        //down
        14, 13, 12,
        15, 14, 13,

        //left
        16, 17, 18,
        18, 19, 17,

        //right
        22, 21, 20,
        23, 22, 21
    };
    m_pIndexBuffer = pDev->CreateIndexBuffer(&index, sizeof(TsInt)* 36);

    m_pTransformCBuffer = TsNew(TsTransformCBuffer);
    m_pTransform = TsNew(TsTransForm);
    m_pTransformCBuffer->SetTransform(m_pTransform);
    m_pTransformCBuffer->CreateTransformCBuffer(pDev);

    return TS_TRUE;
}

TsBool TsSkyBox::Draw(TsDeviceContext* context)
{
    context->SetTopology(D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->DrawIndex(36, 0);
    return TS_TRUE;
}

TsBool TsSkyBox::ApplyVertexBuffer(TsDeviceContext* context)
{
    if (context && m_pVertexBuffer)
        context->SetVertexBuffer(m_pVertexBuffer);        
    else
        return TS_FALSE;
    return TS_TRUE;
}

TsBool TsSkyBox::ApplyIndexBuffer(TsDeviceContext* context)
{
    if (context && m_pIndexBuffer)
        context->SetIndexBuffer(m_pIndexBuffer);
    else
        return TS_FALSE;
    return TS_TRUE;
}

TsBool TsSkyBox::UpdateTransform(TsDeviceContext* context)
{
    m_pTransformCBuffer->UpdateCBuffer(context);

    return TS_TRUE;
}

TsBool TsSkyBox::ApplyTransForm(TsDeviceContext* context)
{    
    if (context && m_pTransformCBuffer)
        m_pTransformCBuffer->ApplyCBuffer(context);
    else
        return TS_FALSE;
    return TS_TRUE;
}

TsBool TsSkyBox::ApplyMaterial(TsDeviceContext* context)
{
    if (context)
    {
        context->SetTexture( 0 ,m_pCubeMap ,TS_SHADER_TYPE::PIXEL_SHADER);
        context->SetSamplerState(
            TsResourceManager::Find<TsSamplerState>("Cube"), 
            0, 
            TS_SHADER_TYPE::PIXEL_SHADER);
    }
    else
        return TS_FALSE;
    return TS_TRUE;
}

//bind cube map
TsBool TsSkyBox::SetCubeMap(TsCubeMap* pCubeMap)
{
    m_pCubeMap = pCubeMap;
    return TS_TRUE;
}