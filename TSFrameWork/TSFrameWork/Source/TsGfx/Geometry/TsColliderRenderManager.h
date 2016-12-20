//**********************************************************
//! TsColliderRenderManager.h
//! コライダレンダラを最適化して描画するマネージャ.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

//=========================================================================
//! TsPlaneGeometry 
//=========================================================================
class TsColliderRenderManager :public TsDrawObject
{
public:
    //=========================================================================
    //! public method
    //=========================================================================

    //! Constructor
    TsColliderRenderManager();

    //! Destructor
    virtual ~TsColliderRenderManager();

    TsBool Initialize(TsDevice* pDev);

    TsBool  AddGeometory( TsCollider* pCollider,
                          TsTransForm* pParent = nullptr,
                          TsInt materialIndex = 0);
    TsBool RemoveGeometory(const TsCollider* pCollider);

    const TsTransForm * FindTransform(TsCollider *)const;

    TsBool ChangeMaterial(TsCollider* pCollider,
                          TsInt materialIndex);

    TsBool CreateMaterial( TsDevice * pDev, TsColor color );

    virtual TsBool Draw(TsDeviceContext* pDC)override;
private:
    
    //----------------------------------------------------------
    // Define
    //----------------------------------------------------------
    static const TsInt MAX_OBJECT = 8192;

    struct Geometory
    {
        TsTransForm* pTransform;
        TsCollider*  pCollider;
    };

    struct MaterialType
    {
        TsVector<Geometory> m_geometorys;
    };

    struct GeometoryType
    {
        TsVector<MaterialType> m_materials;
    };

    //----------------------------------------------------------
    //! Transform専用　簡易アロケーター
    //----------------------------------------------------------
    class TransformAllocator
    {
    public:
        //! Constructor
        TransformAllocator();

        //! メモリ確保
        TsTransForm* Malloc();

        //! メモリ解放
        void Free(TsTransForm* ptr);
    private:
        TsTransForm memory[MAX_OBJECT];
        TsBool      usingFlag[MAX_OBJECT];
    };

    //=========================================================================
    //! propery
    //=========================================================================
    TransformAllocator m_allocator;

    TsVector<TsDefaultMaterial*> m_materialList;
    TsMap<TsCollider::eType, TsVertexBuffer*> m_geometyMap;

    TsVector<TsColliderRenderObject*> m_pLine2DGeometory;
    TsVector<TsColliderRenderObject*> m_pLine3DGeometory;

    GeometoryType m_geometoris[TsCollider::eType::Collider_Num];
    TsInstanceCBuffer* m_pInstanceCB;

    TsRasterizerState   * m_pRasterState;
    TsDepthStencilState * m_pDepthStencilState;
    TsBool                m_useWireFrame;
};