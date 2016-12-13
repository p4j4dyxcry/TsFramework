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
m_vertexCount(0),
m_pDepthStencilState(nullptr),
m_pRasterState(nullptr),
m_useWireFrame(TS_TRUE)
{

}

//=========================================================================
//! Destructor
//=========================================================================
TsColliderRenderObject::~TsColliderRenderObject()
{
    TsSafeDelete(m_pVertex);
    TsSafeDelete(m_transformCBuffer);
    TsSafeDelete(m_pMaterial);
}

//=========================================================================
//! Create　Geometry
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
        m_pMaterial->SetColor(TsFloat4(0, 1, 0, 1));
    }

    if (m_pRasterState == nullptr)
    {
        m_pRasterState = TsNew(TsRasterizerState);
        m_pRasterState->SetAntiAliasMode(TS_TRUE);
        m_pRasterState->SetCullMode(TS_CULL_MODE::NONE);
        m_pRasterState->SetFillMode(TS_FILL_MODE::WIREFRAME);
        m_pRasterState->CreateRasterizerState(pDev);
    }

    if (m_pDepthStencilState == nullptr)
    {
        m_pDepthStencilState = TsNew(TsDepthStencilState);
        m_pDepthStencilState->SetDepthTestFunc(TS_COMPARISON_FUNC::LESS);
        m_pDepthStencilState->SetZEnable(TS_TRUE);
        m_pDepthStencilState->SetZWriteEnable(TS_TRUE);
        m_pDepthStencilState->CreateDepthStencil(pDev);
    }

    return TS_TRUE;
}

//=========================================================================
//! Create　Collider
//=========================================================================
TsBool TsColliderRenderObject::CreateRenderObject(TsDevice* pDev,
                                                  TsCollider* pCollider)
{
    m_pCollider = pCollider;
    Create(pDev);

    //頂点の設定をする
    SetTopology(pCollider->GetType());

    //頂点を作成する
    CreateVertexBuffer(pDev, pCollider);

    //コライダ用の姿勢を作成する
    CreateGeomtoricTransform(pCollider);

    return TS_TRUE;
}
//=========================================================================
//! 頂点トポロジの設定を行う
//=========================================================================
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
    default:
        m_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        break;
    }
}

//----------------------------------------------------------
//! 形状に合わせた頂点バッファの作成
//----------------------------------------------------------
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
        m_pVertex = CreateBox2DVertex();
        break;

    case TsCollider::Collider_AABB3D:
    case TsCollider::Collider_OBB3D:
        m_pVertex = CreateBoxVertex();
        break;
    case TsCollider::Collider_TsSphere:
    case TsCollider::Collider_TsCircle:
        m_pVertex = CreateSphereVertex();
        break;        

    default:
        return TS_FALSE;
    }

    m_pVertexBuffer = 
        pDev->CreateVertexBuffer(m_pVertex, 
                                 sizeof(TsVertexSkin)* m_vertexCount, 
                                 sizeof(TsVertexSkin));

    return TS_TRUE;
}

//----------------------------------------------------------
//! 形状の姿勢を作成する
//----------------------------------------------------------
TsBool  TsColliderRenderObject::CreateGeomtoricTransform(TsCollider* pCollider)
{
    m_geometoricTransform = pCollider->GetGeometoricMatrix();
    m_transformCBuffer->SetTransform(&m_geometoricTransform);

    //行列計算モードは Translate * Rotate * Scale に変更する
//    m_transformCBuffer->SetMatrixConvertOrder(TsTransformCBuffer::MatrixConvertOrder::MTX_CVT_TRS);

    return TS_TRUE;
}

//----------------------------------------------------------
//! 線分メッシュを作成する
//----------------------------------------------------------
template<typename T>
TsVertexSkin* TsColliderRenderObject::CreateLineVertex( TsLine<T>* pLine )
{
    TsVertexSkin * pVertex = nullptr; 

    m_vertexCount = 2;
    pVertex = TsNew(TsVertexSkin[m_vertexCount]);
    memset(pVertex, 0, sizeof(TsVertexSkin)* m_vertexCount);

    TsF32 lineSize = sizeof(T) / sizeof(TsF32);
    const T&  begin = pLine->GetBegin();
    const T&  end = pLine->GetEnd();
    for (TsUint i = 0; i < m_vertexCount; ++i)
    {
        pVertex[0].pos[i] = begin[i];
        pVertex[1].pos[i] = end[i];
    }

    return pVertex;
}

//----------------------------------------------------------
//! 球メッシュを作成する
//----------------------------------------------------------
TsVertexSkin* TsColliderRenderObject::CreateSphereVertex()
{
    TsVertexSkin * pVertex = nullptr;
    TsPrimitiveMeshCreater::GenerateSphere(&pVertex, (TsInt&)m_vertexCount,10);

    return pVertex;    
}

//----------------------------------------------------------
//! スプライトメッシュを作成する
//----------------------------------------------------------
TsVertexSkin* TsColliderRenderObject::CreateBox2DVertex()
{
    TsVertexSkin * pVertex = nullptr;

    TsPrimitiveMeshCreater::GenerateSprite(&pVertex, (TsInt&)m_vertexCount,TS_CULL_MODE::NONE);

    return pVertex;
}

//----------------------------------------------------------
//! 立方体メッシュを作成する
//----------------------------------------------------------
TsVertexSkin* TsColliderRenderObject::CreateBoxVertex()
{
    TsVertexSkin * pVertex = nullptr;

    TsPrimitiveMeshCreater::GenerateBox3D(&pVertex, (TsInt&)m_vertexCount);

    return pVertex;
}

//=========================================================================
//! Draw Call
//=========================================================================
TsBool TsColliderRenderObject::Draw(TsDeviceContext* context)
{
    if (m_pVertexBuffer)
    {
        auto contextDSS = context->GetDepthStencilState();
        auto contextRSS = context->GetRasterState();

        if (m_useWireFrame)
        {
            context->SetDepthStencilState(m_pDepthStencilState);
            context->SetRasterizerState(m_pRasterState);

            context->ApplyDepthStencil();
            context->ApplyRasterizer();
        }
        context->SetTopology(m_topology);
        context->Draw(m_vertexCount, 0);

        if (m_useWireFrame)
        {
            context->SetDepthStencilState(contextDSS);
            context->SetRasterizerState(contextRSS);

            context->ApplyDepthStencil();
            context->ApplyRasterizer();
        }

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
    CreateGeomtoricTransform(m_pCollider);
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
    m_geometoricTransform.SetParent(pTransform);
    return TS_TRUE;
}

//----------------------------------------------------------
//! 色を設定する
//----------------------------------------------------------
TsBool TsColliderRenderObject::SetColor(TsF32 r, TsF32 g, TsF32 b, TsF32 a)
{
    return SetColor(TsFloat4(r, g, b, a));
}

//----------------------------------------------------------
//! 色を設定する
//----------------------------------------------------------
TsBool TsColliderRenderObject::SetColor(const TsFloat4& color)
{
    m_pMaterial->SetColor(color);
    return TS_TRUE;
}

//----------------------------------------------------------
//! 色を設定する
//----------------------------------------------------------
TsBool TsColliderRenderObject::SetColor(const TsColor& color)
{
    return SetColor(color.m_color);
}