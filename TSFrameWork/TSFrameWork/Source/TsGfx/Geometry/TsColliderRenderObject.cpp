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
    case TsCollider::Collider_AABB2D:
    case TsCollider::Collider_AABB3D:
    case TsCollider::Collider_OBB3D:
    case TsCollider::Collider_TsSphere:
    case TsCollider::Collider_TsCircle:
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
    //各形状をTransformに変換する
    switch (pCollider->GetType())
    {
    case TsCollider::Collider_AABB2D:
        {
            TsAABB2D * pAABB2D = ((TsAABB2D*)(pCollider));
            TsVector3 scale = pAABB2D->GetMax() - pAABB2D->GetMin();
            TsVector3 translate = pAABB2D->GetCenter();
            m_geometoricTransform.m_localScale = scale;
            m_geometoricTransform.m_localPosition = translate;
            break;

        }
    case TsCollider::Collider_AABB3D:
        {
            TsAABB3D * pAABB3D = ((TsAABB3D*)(pCollider));
            TsVector3 scale = pAABB3D->GetMax() - pAABB3D->GetMin();
            TsVector3 translate = pAABB3D->GetCenter();
            m_geometoricTransform.m_localScale = scale;
            m_geometoricTransform.m_localPosition = translate;
            break;
        }
    case TsCollider::Collider_OBB3D:
        {
            TsOBB * pOBB = ((TsOBB*)(pCollider));
            TsVector3 scale = pOBB->GetScale();
            TsVector3 translate = pOBB->GetCenter();
            TsQuaternion rotate = pOBB->GetRotate();

            m_geometoricTransform.m_localScale = scale*2;
            m_geometoricTransform.m_localPosition = translate;
            m_geometoricTransform.m_localRotate = rotate;
            break;
        }
    case TsCollider::Collider_TsSphere:
        {
            TsSphere3D * pSphere = ((TsSphere3D*)(pCollider));
            TsF32 r = pSphere->GetRadius();
            TsVector3 scale = TsVector3( r , r , r );
            TsVector3 translate = pSphere->GetCenter();

            m_geometoricTransform.m_localScale = scale;
            m_geometoricTransform.m_localPosition = translate;
            break;
        }
    case TsCollider::Collider_TsCircle:
        {
            TsSphere2D * pSphere = ((TsSphere2D*)(pCollider));
            TsF32 r = pSphere->GetRadius();
            TsVector3 scale = TsVector3( r , r , r );
            TsVector3 translate = pSphere->GetCenter();

            m_geometoricTransform.m_localScale = scale;
            m_geometoricTransform.m_localPosition = translate;
            break;
        }
    default:
        break;
    }
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
    TsVertexSkin * pVertex;

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
    TsVertexSkin * pVertex;
    TsSphereMeshCreater creater;

    TsInt div = 12;  //球の分割数

    creater.CreateSphere(div);
    auto posList = creater.GetPositions();
    auto nomList = creater.GetNomals();
    auto index = creater.GetIndex();

    m_vertexCount = index.size();
    if (index.empty())
        return nullptr;

    pVertex = TsNew(TsVertexSkin[m_vertexCount]);
    memset(pVertex, 0, sizeof(TsVertexSkin)* m_vertexCount);
    for (TsUint i = 0; i < m_vertexCount; ++i)
    {
        pVertex[i].pos = posList[index[i]];
        pVertex[i].normal = nomList[index[i]];
    }

    return pVertex;    
}

//----------------------------------------------------------
//! スプライトメッシュを作成する
//----------------------------------------------------------
TsVertexSkin* TsColliderRenderObject::CreateBox2DVertex()
{
    TsVertexSkin * pVertex = nullptr;

    m_vertexCount = 12;

    pVertex = TsNew(TsVertexSkin[m_vertexCount]);

    memset(pVertex, 0, sizeof(TsVertexSkin)* m_vertexCount);

    TsVector3 left_up = TsVector3(-0.5, 0.5, 0);
    TsVector3 left_down = TsVector3(-0.5, -0.5, 0);
    TsVector3 right_up = TsVector3(0.5, 0.5, 0);
    TsVector3 right_down = TsVector3(0.5, -0.5, 0);

    //前の面
    pVertex[0].pos = left_up;
    pVertex[1].pos = right_up;
    pVertex[2].pos = left_down;

    pVertex[3].pos = right_up;
    pVertex[4].pos = left_down;
    pVertex[5].pos = right_down;

    //後ろの面
    pVertex[6].pos = right_up;
    pVertex[7].pos = left_up;
    pVertex[8].pos = right_down;

    pVertex[9].pos = left_up;
    pVertex[10].pos = right_down;
    pVertex[11].pos = left_down;


    return pVertex;
}

//----------------------------------------------------------
//! 立方体メッシュを作成する
//----------------------------------------------------------
TsVertexSkin* TsColliderRenderObject::CreateBoxVertex()
{
    TsVertexSkin * pVertex = nullptr;

    m_vertexCount = 36;

    pVertex = TsNew(TsVertexSkin[m_vertexCount]);

    memset(pVertex, 0, sizeof(TsVertexSkin)* m_vertexCount);


    TsVector3 left_up_back      = TsVector3(-0.5, 0.5, 0.5);
    TsVector3 left_down_back    = TsVector3(-0.5,-0.5, 0.5);
    TsVector3 left_up_front     = TsVector3(-0.5, 0.5,-0.5);
    TsVector3 left_down_front   = TsVector3(-0.5,-0.5,-0.5);
    TsVector3 right_up_back     = TsVector3( 0.5, 0.5, 0.5);
    TsVector3 right_down_back   = TsVector3( 0.5,-0.5, 0.5);
    TsVector3 right_up_front    = TsVector3( 0.5, 0.5,-0.5);
    TsVector3 right_down_front  = TsVector3( 0.5,-0.5,-0.5);

    //法線を設定する
    for (TsInt i = 0; i < 6; ++i)
        pVertex[i].normal = TsVector3::up;
    for (TsInt i = 0; i < 6; ++i)
        pVertex[6+i].normal = TsVector3::down;
    for (TsInt i = 0; i < 6; ++i)
        pVertex[12+i].normal = TsVector3::left;
    for (TsInt i = 0; i < 6; ++i)
        pVertex[18+i].normal = TsVector3::right;
    for (TsInt i = 0; i < 6; ++i)
        pVertex[24+i].normal = TsVector3::front;
    for (TsInt i = 0; i < 6; ++i)
        pVertex[30 + i].normal = TsVector3::back;

    //上の面
    pVertex[0].pos = left_up_back;
    pVertex[1].pos = right_up_back;
    pVertex[2].pos = left_up_front;

    pVertex[3].pos = right_up_back;
    pVertex[4].pos = right_up_front;
    pVertex[5].pos = left_up_front;

    //下の面
    pVertex[6].pos = right_down_back;
    pVertex[7].pos = left_down_back;
    pVertex[8].pos = left_down_front;

    pVertex[9].pos = right_down_back;
    pVertex[10].pos = left_down_front;
    pVertex[11].pos = right_down_front;

    //左の面
    pVertex[12].pos = left_up_back;
    pVertex[13].pos = left_up_front;
    pVertex[14].pos = left_down_back;

    pVertex[15].pos = left_up_front;
    pVertex[16].pos = left_down_front;
    pVertex[17].pos = left_down_back;

    //右の面
    pVertex[18].pos = right_up_front;
    pVertex[19].pos = right_up_back;
    pVertex[20].pos = right_down_front;

    pVertex[21].pos = right_up_back;
    pVertex[22].pos = right_down_back;
    pVertex[23].pos = right_down_front;

    //前の面
    pVertex[24].pos = left_up_front;
    pVertex[25].pos = right_up_front;
    pVertex[26].pos = left_down_front;

    pVertex[27].pos = right_up_front;
    pVertex[28].pos = right_down_front;
    pVertex[29].pos = left_down_front;
    //後ろの面
    pVertex[30].pos = right_up_back;
    pVertex[31].pos = left_up_back;
    pVertex[32].pos = right_down_back;

    pVertex[33].pos = left_up_back;
    pVertex[34].pos = left_down_back;
    pVertex[35].pos = right_down_back;


    return pVertex;
}

//=========================================================================
//! Draw Call
//=========================================================================
TsBool TsColliderRenderObject::Draw(TsDeviceContext* context)
{
    if (m_pVertexBuffer)
    {
        context->SetTopology(m_topology);
        context->Draw(m_vertexCount, 0);
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