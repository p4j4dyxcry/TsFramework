//**********************************************************
//! TsColliderRenderManager.h
//! コライダレンダラを最適化して描画するマネージャ.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

//=========================================================================
//! TsPlaneGeometry 
//=========================================================================
class TsColliderRenderManager
{
public:
    //=========================================================================
    //! public method
    //=========================================================================

    //! Constructor
    TsColliderRenderManager();

    //! Destructor
    virtual ~TsColliderRenderManager();

    TsBool  AddGeometory( const TsCollider& collider ,
                          TsTransForm* pParent = nullptr);

private:
    //=========================================================================
    //! propery
    //=========================================================================

    TsColliderRenderObject* m_pSphereGeometory;
    TsColliderRenderObject* m_pCircleGeometory;

    TsColliderRenderObject* m_pBoxGeometory;
    TsColliderRenderObject* m_pSpriteGeometory;

    TsVector<TsColliderRenderObject*> m_pLine2DGeometory;
    TsVector<TsColliderRenderObject*> m_pLine3DGeometory;

    TsVector<TsTransForm*>   m_spheres;
    TsVector<TsTransForm*>   m_cirles;

    TsVector<TsTransForm*>   m_AABB2Ds;
    TsVector<TsTransForm*>   m_AABB3Ds;

    TsVector<TsTransForm*>   m_OBBs;

    TsVector<TsTransForm*>   m_Line2Ds;
    TsVector<TsTransForm*>   m_Line3Ds;

    TsVertexShader *     m_pinstancingVertexShader;
};