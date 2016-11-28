#include "../TsGfx.h"

//=========================================================================
//! Constructor
//=========================================================================
TsColliderRenderObject::TsColliderRenderObject() :
m_pVertexBuffer(nullptr),
m_transformCBuffer(nullptr),
m_pMaterial(nullptr)
{
    //m_vertex[0].pos = TsVector3(-.5f, 0, .5f);
    //m_vertex[1].pos = TsVector3(.5f, 0, .5f);
    //m_vertex[2].pos = TsVector3(-.5f, 0, -.5f);
    //m_vertex[3].pos = TsVector3(.5f, 0, -.5f);

    //for (TsInt i = 0; i < 4; ++i)
    //{
    //    m_vertex[i].boneIndex = TsInt4(0, 0, 0, 0);
    //    m_vertex[i].weight = TsVector4::zero;
    //    m_vertex[i].normal = TsVector3::up;

    //    // x0y0 x1y0 x0y1 x1y1 
    //    m_vertex[i].uv = TsVector2((TsF32)(i % 2), (TsF32)(i / 2));
    //}
}

//=========================================================================
//! Destructor
//=========================================================================
TsColliderRenderObject::~TsColliderRenderObject()
{
    TsSafeDelete(m_pVertex);
    TsSafeDelete(m_pVertex);
    TsSafeDelete(m_transformCBuffer);
    TsSafeDelete(m_pMaterial);
}

//=========================================================================
//! Create@Geometry
//=========================================================================
TsBool TsColliderRenderObject::Create(TsDevice* pDev)
{
    //m_pVertexBuffer = pDev->CreateVertexBuffer(m_vertex, sizeof(m_vertex), sizeof(TsVertexSkin), 0);
    //m_transformCBuffer = TsNew(TsTransformCBuffer);

    //m_transformCBuffer->CreateTransformCBuffer(pDev);
    //m_pMaterial = TsNew(TsDefaultMatrial);
    //m_pMaterial->CreateMaterial(pDev);
    //m_pMaterial->SetAmbient(TsFloat4(1, 1, 1, 1));
    return TS_TRUE;
}

//=========================================================================
//! Draw Call
//=========================================================================
TsBool TsColliderRenderObject::Draw(TsDeviceContext* context)
{
    if (m_pVertexBuffer)
    {
        context->SetTopology(D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        context->Draw(4, 0);
    }
    else
    {
        return TS_FALSE;
    }
    return TS_TRUE;
}

//=========================================================================
//! Apply Vertex Buffer
//=========================================================================
TsBool TsColliderRenderObject::ApplyVertexBuffer(TsDeviceContext* context)
{
    if (m_pVertexBuffer)
        context->SetVertexBuffer(m_pVertexBuffer);
    else
        return TS_FALSE;
    return TS_TRUE;
}

//=========================================================================
//! Update Transform
//=========================================================================
TsBool TsColliderRenderObject::UpdateTransform(TsDeviceContext* context)
{
    if (m_transformCBuffer)
        m_transformCBuffer->UpdateCBuffer(context);
    else
        return TS_FALSE;
    return TS_TRUE;
}

//=========================================================================
//! Apply Transform
//=========================================================================
TsBool TsColliderRenderObject::ApplyTransForm(TsDeviceContext* context)
{
    if (m_transformCBuffer)
        m_transformCBuffer->ApplyCBuffer(context);
    else
        return TS_FALSE;
    return TS_TRUE;
}

//=========================================================================
//! Update Material
//=========================================================================
TsBool TsColliderRenderObject::UpdateMaterial(TsDeviceContext* context)
{
    if (m_pMaterial)
        m_pMaterial->UpdateMaterial(context);
    else
        return TS_FALSE;
    return TS_TRUE;
}

//=========================================================================
//! Apply Material
//=========================================================================
TsBool TsColliderRenderObject::ApplyMaterial(TsDeviceContext* context)
{
    if (m_pMaterial)
        m_pMaterial->ApplyMaterial(context);
    else
        return TS_FALSE;
    return TS_TRUE;
}

//=========================================================================
//! Set Transform
//=========================================================================
TsBool TsColliderRenderObject::SetTransform(TsTransForm* pTransform)
{
    if (m_transformCBuffer)
        m_transformCBuffer->SetTransform(pTransform);
    else
        return TS_FALSE;
    return TS_TRUE;
}