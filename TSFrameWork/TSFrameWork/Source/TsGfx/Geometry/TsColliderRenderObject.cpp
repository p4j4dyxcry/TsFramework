#include "../TsGfx.h"

//=========================================================================
//! Constructor
//=========================================================================
TsColliderRenderObject::TsColliderRenderObject() :
m_pVertexBuffer(nullptr),
m_transformCBuffer(nullptr),
m_pMaterial(nullptr),
m_pCollider(nullptr),
m_topology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST),
m_vertexCount(0)
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
    if (m_transformCBuffer == nullptr)
    {
        m_transformCBuffer = TsNew(TsTransformCBuffer);
        m_transformCBuffer->CreateTransformCBuffer(pDev);
    }

    if (m_pMaterial == nullptr)
    {
        m_pMaterial = TsNew(TsDefaultMatrial);
        m_pMaterial->CreateMaterial(pDev);
        m_pMaterial->SetAmbient(TsFloat4(1, 1, 1, 1));
    }
    return TS_TRUE;
}

//=========================================================================
//! Create@Collider
//=========================================================================
TsBool TsColliderRenderObject::CreateRenderObject(TsDevice* pDev,
                                                  TsCollider* pCollider)
{
    Create(pDev);

    SetTopology(pCollider->GetType());

    return TS_TRUE;
}
void TsColliderRenderObject::SetTopology(TsCollider::eType type)
{
    switch ( type )
    {
    case TsCollider::Collider_Line2D:
    case TsCollider::Collider_Line3D:
    case TsCollider::Collider_Ray2D:
    case TsCollider::Collider_Ray3D:
        m_topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
        break;
    case TsCollider::Collider_AABB2D:
    case TsCollider::Collider_AABB3D:
    case TsCollider::Collider_OBB3D:
    case TsCollider::Collider_TsSphere:
    case TsCollider::Collider_TsCircle:
        m_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        break;
    }
}

TsBool  TsColliderRenderObject::CreateVertexBuffer(TsDevice* pDev, TsCollider* pCollider)
{
    switch (pCollider->GetType())
    {
    case TsCollider::Collider_Line2D:
    case TsCollider::Collider_Ray2D:
        m_pVertex = CreateLineVertex((TsLine2D*)(pCollider));
        break;
    case TsCollider::Collider_Line3D:
    case TsCollider::Collider_Ray3D:
        m_pVertex = CreateLineVertex((TsLine3D*)(pCollider));
        break;
    case TsCollider::Collider_AABB2D:
        break;

    case TsCollider::Collider_AABB3D:
    case TsCollider::Collider_OBB3D:
        break;
    case TsCollider::Collider_TsSphere:
        m_pVertex = CreateSphereVertex();
        break;        
    case TsCollider::Collider_TsCircle:
        break;
    default:
        return TS_FALSE;
    }

    return TS_TRUE;
}

template<typename T>
TsVertexSkin* TsColliderRenderObject::CreateLineVertex( TsLine<T>* pLine )
{
    TsVertexSkin * pVertex;

    m_vertexCount = 2;
    pVertex = TsNew(TsVertexSkin[2]);
    memset(m_pVertex, 0, sizeof(TsVertexSkin)* m_vertexCount);

    TsF32 lineSize = sizeof(T) / sizeof(TsF32);
    const T&  begin = pLine->GetBegin();
    const T&  end = pLine->GetEnd();
    for (TsUint i = 0; i < m_vertexCount; ++i)
    {
        m_pVertex[0].pos[i] = begin[i];
        m_pVertex[1].pos[i] = end[i];
    }

    return pVertex;
}

TsVertexSkin* TsColliderRenderObject::CreateSphereVertex()
{
    TsVertexSkin * pVertex;
    TsSphereMeshCreater creater;
    creater.CreateSphere(8);
    auto posList = creater.GetPositions();
    auto nomList = creater.GetNomals();
    auto index = creater.GetIndex();

    m_vertexCount = index.size();
    if (index.empty())
        return nullptr;

    pVertex = TsNew(TsVertexSkin[index.size()]);
    memset(pVertex, 0, sizeof(TsVertexSkin)* m_vertexCount);
    for (TsUint i = 0; i < m_vertexCount; ++i)
    {
        pVertex[i].pos = posList[index[i]];
        pVertex[i].normal = posList[index[i]];
    }

    return pVertex;    
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