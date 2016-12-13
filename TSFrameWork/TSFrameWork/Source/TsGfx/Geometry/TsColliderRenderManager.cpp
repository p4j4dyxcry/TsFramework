#include "../TsGfx.h"

TsColliderRenderManager::TransformAllocator::TransformAllocator()
{
    memset(usingFlag, 0, sizeof(TsBool) * MAX_OBJECT);
}

TsTransForm* TsColliderRenderManager::TransformAllocator::Malloc()
{
    //! ÉÅÉÇÉäãÛÇ´ÇíTÇ∑ÅB
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
}

//! ÉÅÉÇÉäâï˙
void TsColliderRenderManager::TransformAllocator::Free(TsTransForm* ptr)
{
    ptr->~TsTransForm();
    TsInt ptrDiff = ptr - &memory[0];
    usingFlag[ptrDiff] = TS_FALSE;
}


TsColliderRenderManager::TsColliderRenderManager()
{
    m_pLine2DGeometory.reserve(1024);
    m_pLine3DGeometory.reserve(1024);
}

TsColliderRenderManager::~TsColliderRenderManager()
{
    for (auto p : m_materialList)
        TsSafeDelete(p);
    for (auto p : m_geometyMap)
        TsSafeDelete(p.second);
    TsSafeDelete(m_pInstanceCB);
}

TsBool TsColliderRenderManager::Initialize(TsDevice* pDev)
{
    TsPrimitiveMeshCreater creater;

    //! AABB2D
    {
        TsVertexSkin* pSkin = nullptr;
        TsInt sz;
        creater.GenerateSprite(&pSkin, sz, TS_CULL_MODE::NONE);

        TsVertexBuffer* pVB = pDev->CreateVertexBuffer(pSkin, sz*sizeof(TsVertexSkin), sizeof(TsVertexSkin));
            

        tstl::pair<TsCollider::eType, TsVertexBuffer*> pair;
        pair.first = TsCollider::eType::Collider_AABB2D;
        pair.second = pVB;
        m_geometyMap.insert(pair);
    }

    //! AABB3D
    {
        TsVertexSkin* pSkin = nullptr;
        TsInt sz;
        creater.GenerateBox3D(&pSkin, sz);

        TsVertexBuffer* pVB = pDev->CreateVertexBuffer(pSkin, sz*sizeof(TsVertexSkin), sizeof(TsVertexSkin));

        tstl::pair<TsCollider::eType, TsVertexBuffer*> pair;
        pair.first = TsCollider::eType::Collider_AABB3D;
        pair.second = pVB;
        m_geometyMap.insert(pair);
    }

    //! OBB
    {
        TsVertexSkin* pSkin = nullptr;
        TsInt sz;
        creater.GenerateBox3D(&pSkin, sz);

        TsVertexBuffer* pVB = pDev->CreateVertexBuffer(pSkin, sz*sizeof(TsVertexSkin), sizeof(TsVertexSkin));

        tstl::pair<TsCollider::eType, TsVertexBuffer*> pair;
        pair.first = TsCollider::eType::Collider_OBB3D;
        pair.second = pVB;
        m_geometyMap.insert(pair);
    }

    //! Sphere
    {
        TsVertexSkin* pSkin = nullptr;
        TsInt sz;
        creater.GenerateSphere(&pSkin, sz,32);

        TsVertexBuffer* pVB = pDev->CreateVertexBuffer(pSkin, sz*sizeof(TsVertexSkin), sizeof(TsVertexSkin));

        tstl::pair<TsCollider::eType, TsVertexBuffer*> pair;
        pair.first = TsCollider::eType::Collider_TsSphere;
        pair.second = pVB;
        m_geometyMap.insert(pair);
    }
    m_pInstanceCB = TsNew(TsInstanceCBuffer);

    m_pInstanceCB->CreateInstanceCBuffer(pDev);

    CreateMaterial(pDev, TsColor::Green);
    CreateMaterial(pDev, TsColor::Red);

    return TS_TRUE;
}

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

TsBool TsColliderRenderManager::CreateMaterial(TsDevice* pDev, TsColor color)
{
    TsDefaultMatrial * m = TsNew(TsDefaultMatrial);
    m->CreateMaterial(pDev);
    m->SetColor(color.m_color);

    m_materialList.push_back(m);
    for (auto it : m_geometyMap)
    {
        m_geometoris[it.first].m_materials.push_back(MaterialType());
    }
    return TS_TRUE;
}

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

TsBool TsColliderRenderManager::Draw(TsDeviceContext* pDC)
{
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
                pDC->SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                pDC->SetVertexBuffer(it.second);
//                pDC->Draw(it.second->GetBufferSize() / sizeof(TsVertexSkin));
                pDC->DrawInstance(it.second->GetVertexCount(), m.m_geometorys.size());
            }
        }
    }
    return TS_TRUE;
};