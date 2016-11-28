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
protected:
    //----------------------------------------------------------
    // peropery
    //----------------------------------------------------------
    TsVertexBuffer*         m_pVertexBuffer;
    TsVertexSkin*           m_pVertex;
    TsTransformCBuffer*     m_transformCBuffer;
    TsDefaultMatrial*       m_pMaterial;
};