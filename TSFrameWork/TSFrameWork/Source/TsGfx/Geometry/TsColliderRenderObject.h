//!*******************************************************
//! TsColliderRenderObject.h
//!
//! コライダを視覚化します.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsColliderRenderObject : public TsDrawObject
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsColliderRenderObject();
    virtual ~TsColliderRenderObject();

    virtual TsBool Create(TsDevice* pDev)override;

    virtual TsBool Draw(TsDeviceContext * pDevContext)override;
    virtual TsBool ApplyVertexBuffer(TsDeviceContext* pDevContext)override;
    virtual TsBool UpdateTransform(TsDeviceContext* pDevContext)override;
    virtual TsBool ApplyTransForm(TsDeviceContext* pDevContext)override;

    virtual TsBool UpdateMaterial(TsDeviceContext* pDevContext)override;
    virtual TsBool ApplyMaterial(TsDeviceContext* pDevContext)override;

    TsBool SetTransform(TsTransForm* pTransform);
    TsBool CreateRenderObject(TsDevice* pDev , TsCollider* pCollider);

    TsBool SetColor(TsF32 r, TsF32 g, TsF32 b , TsF32 a);
    TsBool SetColor(const TsFloat4& color);
    TsBool SetColor(const TsColor& color);

protected:

    void    SetTopology(TsCollider::eType colliderType);
    TsBool  CreateVertexBuffer( TsDevice* pDev, TsCollider* );
    TsBool  CreateGeomtoricTransform(TsCollider* pCollider);

    template<typename T>
    TsVertexSkin* CreateLineVertex( TsLine<T>* pLine );

    TsVertexSkin* CreateSphereVertex();
    TsVertexSkin* CreateCapsule3DVertex();
    TsVertexSkin* CreateBoxVertex();
    TsVertexSkin* CreateBox2DVertex();

    //----------------------------------------------------------
    // peropery
    //----------------------------------------------------------
    TsVertexBuffer*         m_pVertexBuffer;
    TsVertexSkin*           m_pVertex;
    TsTransformCBuffer*     m_transformCBuffer;
    TsTransForm             m_geometoricTransform;
    TsDefaultMaterial*       m_pMaterial;

    TsCollider *            m_pCollider;    
    D3D_PRIMITIVE_TOPOLOGY  m_topology;
    TsUint                  m_vertexCount;
    TsBool                  m_useWireFrame;
    TsRasterizerState*      m_pRasterState;
    TsDepthStencilState*    m_pDepthStencilState;
};