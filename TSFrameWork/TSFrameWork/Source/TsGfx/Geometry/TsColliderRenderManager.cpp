#include "../TsGfx.h"

//----------------------------------------------------------
//! コンストラクタ
//----------------------------------------------------------
TsColliderRenderManager::TransformAllocator::TransformAllocator()
{
    memset(usingFlag, 0, sizeof(TsBool) * MAX_OBJECT);
}

//----------------------------------------------------------
//! メモリの確保
//----------------------------------------------------------
TsTransForm* TsColliderRenderManager::TransformAllocator::Malloc()
{
    //! メモリ空きを探す。
    TsInt index = rand() % MAX_OBJECT;
    TsInt count = 0;
    for (TsInt count = 0; count < MAX_OBJECT; ++count)
    {
        if (index == MAX_OBJECT - 1)
            index = 0;

        if (!usingFlag[index])
        {
            usingFlag[index] = TS_TRUE;
            return &memory[index];
        }
        ++index;
    }
    return TS_FALSE;
}

//----------------------------------------------------------
//! メモリの解放
//----------------------------------------------------------
void TsColliderRenderManager::TransformAllocator::Free(TsTransForm* ptr)
{
    ptr->~TsTransForm();
    TsInt ptrDiff = ptr - &memory[0];
    usingFlag[ptrDiff] = TS_FALSE;
}

//----------------------------------------------------------
//! コンストラクタ
//----------------------------------------------------------
TsColliderRenderManager::TsColliderRenderManager()
:m_useWireFrame( TS_TRUE ) ,
m_pDepthStencilState(nullptr),
m_pRasterState(nullptr)
{
    m_pLine2DGeometory.reserve(1024);
    m_pLine3DGeometory.reserve(1024);
}

//----------------------------------------------------------
//! デストラクタ
//----------------------------------------------------------
TsColliderRenderManager::~TsColliderRenderManager()
{
    for (auto p : m_materialList)
        TsSafeDelete(p);
    for (auto p : m_geometyMap)
        TsSafeDelete(p.second);
    TsSafeDelete(m_pInstanceCB);

 //   TsSafeDelete( m_pRasterState );
 //   TsSafeDelete( m_pDepthStencilState );

}

//----------------------------------------------------------
//! 初期化
//----------------------------------------------------------
TsBool TsColliderRenderManager::Initialize(TsDevice* pDev)
{
    TsPrimitiveMeshCreater creater;
    (void)creater;
    //! AABB2D
    {
        TsVertexSkin* pSkin = nullptr;
        TsInt sz;
        creater.GenerateSpriteLine(&pSkin, sz);

        TsVertexBuffer* pVB = pDev->CreateVertexBuffer(pSkin, sz*sizeof(TsVertexSkin), sizeof(TsVertexSkin));
            

        std::pair<TsCollider::eType, TsVertexBuffer*> pair;
        pair.first = TsCollider::eType::Collider_AABB2D;
        pair.second = pVB;
        m_geometyMap.insert(pair);
    }

    //! AABB3D
    {
        TsVertexSkin* pSkin = nullptr;
        TsInt sz;
        creater.GenerateBox3DLine( &pSkin , sz );

        TsVertexBuffer* pVB = pDev->CreateVertexBuffer(pSkin, sz*sizeof(TsVertexSkin), sizeof(TsVertexSkin));

        std::pair<TsCollider::eType, TsVertexBuffer*> pair;
        pair.first = TsCollider::eType::Collider_AABB3D;
        pair.second = pVB;
        m_geometyMap.insert(pair);
    }

    //! OBB
    {
        TsVertexSkin* pSkin = nullptr;
        TsInt sz;
        creater.GenerateBox3DLine(&pSkin, sz);

        TsVertexBuffer* pVB = pDev->CreateVertexBuffer(pSkin, sz*sizeof(TsVertexSkin), sizeof(TsVertexSkin));

        std::pair<TsCollider::eType, TsVertexBuffer*> pair;
        pair.first = TsCollider::eType::Collider_OBB3D;
        pair.second = pVB;
        m_geometyMap.insert(pair);
    }

    //! Sphere
    {
        TsVertexSkin* pSkin = nullptr;
        TsInt sz;
        creater.GenerateSphere(&pSkin, sz,9);
        creater.GenerateCapsule3D(&pSkin, sz, 9, 9);
        TsVertexBuffer* pVB = pDev->CreateVertexBuffer(pSkin, sz*sizeof(TsVertexSkin), sizeof(TsVertexSkin));

        std::pair<TsCollider::eType, TsVertexBuffer*> pair;
        pair.first = TsCollider::eType::Collider_TsSphere;
        pair.second = pVB;
        m_geometyMap.insert(pair);
    }
    m_pInstanceCB = TsNew(TsInstanceCBuffer);

    m_pInstanceCB->CreateInstanceCBuffer(pDev);

    CreateMaterial(pDev, TsColor::Green);
    CreateMaterial(pDev, TsColor::Red);

    if( m_pRasterState == nullptr )
    {
        m_pRasterState = TsNew( TsRasterizerState );
        m_pRasterState->SetAntiAliasMode( TS_TRUE );
        m_pRasterState->SetCullMode( TS_CULL_MODE::NONE );
        m_pRasterState->SetFillMode( TS_FILL_MODE::WIREFRAME );
        m_pRasterState->CreateRasterizerState( pDev );
    }

    if( m_pDepthStencilState == nullptr )
    {
        m_pDepthStencilState = TsNew( TsDepthStencilState );
        m_pDepthStencilState->SetDepthTestFunc( TS_COMPARISON_FUNC::LESS );
        m_pDepthStencilState->SetZEnable( TS_TRUE );
        m_pDepthStencilState->SetZWriteEnable( TS_TRUE );
        m_pDepthStencilState->CreateDepthStencil( pDev );
    }

    return TS_TRUE;
}

//----------------------------------------------------------
//! ジオメトリを追加
//----------------------------------------------------------
TsBool TsColliderRenderManager::AddGeometory(TsCollider* pCollider,
                                             TsTransForm* pParent,
                                             TsInt materialIndex )
{
    Geometory geometory;
    geometory.pTransform = m_allocator.Malloc();
    *geometory.pTransform = pCollider->GetGeometoricMatrix();
    geometory.pCollider = pCollider;

    auto& m = m_geometoris[pCollider->GetType()].m_materials;
    if (m.size() <= (unsigned)materialIndex)
    {
        m_allocator.Free(geometory.pTransform);
        return TS_FALSE;
    }
    m[materialIndex].m_geometorys.push_back(geometory);
    if ( pParent )
        geometory.pTransform->SetParent( pParent );
    return TS_TRUE;
}

//----------------------------------------------------------
//! マテリアルを追加
//----------------------------------------------------------
TsBool TsColliderRenderManager::CreateMaterial(TsDevice* pDev, TsColor color)
{
    TsDefaultMaterial * m = TsNew(TsDefaultMaterial);
    m->CreateMaterial(pDev);
    m->SetColor(color.m_color);

    m_materialList.push_back(m);
    for (auto it : m_geometyMap)
    {
        m_geometoris[it.first].m_materials.push_back(MaterialType());
    }
    return TS_TRUE;
}

//----------------------------------------------------------
//! ジオメトリを削除
//----------------------------------------------------------
TsBool TsColliderRenderManager::RemoveGeometory( const TsCollider* pCollider )
{
    auto& m = m_geometoris[pCollider->GetType()].m_materials;
    for (auto i : m)
    {
        for (auto j : i.m_geometorys)
        {
            if (j.pCollider == pCollider)
            {
                m_allocator.Free(j.pTransform);
                return TS_TRUE;
            }
        }
    }
    return TS_FALSE;
}

//----------------------------------------------------------
//! Transformを取得
//----------------------------------------------------------
const TsTransForm * TsColliderRenderManager::FindTransform(TsCollider * pCollider)const
{
    auto& m = m_geometoris[pCollider->GetType()].m_materials;
    for (auto i : m)
    {
        for (auto j : i.m_geometorys)
        {
            if (j.pCollider == pCollider)
            {
                return j.pTransform;
            }
        }
    }
    return nullptr;
}

//----------------------------------------------------------
//! materialを変更
//----------------------------------------------------------
TsBool TsColliderRenderManager::ChangeMaterial(TsCollider * pCollider, TsInt materialIndex)
{
    const TsTransForm * pTransform = FindTransform(pCollider);
    TsTransForm * pParent = nullptr;
    if (pTransform)
        pParent = pTransform->GetParent();
    RemoveGeometory(pCollider);
    AddGeometory(pCollider, pParent, materialIndex);

    return TS_TRUE;

}

//----------------------------------------------------------
//! 描画
//----------------------------------------------------------
TsBool TsColliderRenderManager::Draw(TsDeviceContext* pDC)
{
    auto contextDSS = pDC->GetDepthStencilState();
    auto contextRSS = pDC->GetRasterState();

    if( m_useWireFrame )
    {
        pDC->SetDepthStencilState( m_pDepthStencilState );
        pDC->SetRasterizerState( m_pRasterState );

        pDC->ApplyDepthStencil();
        pDC->ApplyRasterizer();
    }

    //! UpdateCBuffer
    auto UpdateInstanceCB = [this](MaterialType& m)
    {
        for (TsUint i = 0; i < m.m_geometorys.size(); ++i)
        {
            auto& g = m.m_geometorys[i];
            *g.pTransform = g.pCollider->GetGeometoricMatrix();
            m_pInstanceCB->SetMatrix(i, g.pTransform->ToWorldMatrix());
        }
    };

    for (auto it : m_geometyMap)
    {
        for (TsUint i = 0; i < m_materialList.size(); ++i)
        {
            auto& m = m_geometoris[it.first].m_materials[i];

            //! update cbuffer
            UpdateInstanceCB(m_geometoris[it.first].m_materials[i]);
            m_pInstanceCB->UpdateCBuffer(pDC);

            m_materialList[i]->UpdateMaterial(pDC);

            //! apply cbuffer
            m_pInstanceCB->ApplyCBuffer(pDC);
            m_materialList[i]->ApplyMaterial(pDC);

            //! draw call
            if (m.m_geometorys.size() > 0)
            {
                if( it.first == TsCollider::eType::Collider_TsSphere ||
                    it.first == TsCollider::eType::Collider_TsCircle )
                {
                    pDC->SetTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
                }
                else
                {
                    pDC->SetTopology( D3D_PRIMITIVE_TOPOLOGY_LINELIST );
                }
                pDC->SetVertexBuffer(it.second);
                pDC->DrawInstance(it.second->GetVertexCount(), m.m_geometorys.size());
            }
        }
    }

    if( m_useWireFrame )
    {
        pDC->SetDepthStencilState( contextDSS );
        pDC->SetRasterizerState( contextRSS );

        pDC->ApplyDepthStencil();
        pDC->ApplyRasterizer();
    }

    return TS_TRUE;
};