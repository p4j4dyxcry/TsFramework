//**********************************************************
//! TsPlaneObject.h
//! Plane Geometry.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

//=========================================================================
//! TsPlaneGeometry 
//=========================================================================
class TsPlaneObject : public TsDrawObject
{
public:
    //=========================================================================
    //! public method
    //=========================================================================

    //! Constructor
    TsPlaneObject();

    //! Destructor
    virtual ~TsPlaneObject();

    virtual TsBool Create( TsDevice* pDev )override;
    virtual TsBool Draw( TsDeviceContext * pDevContext )override;
    virtual TsBool ApplyVertexBuffer( TsDeviceContext* pDevContext )override;
    virtual TsBool UpdateTransform( TsDeviceContext* pDevContext )override;
    virtual TsBool ApplyTransForm( TsDeviceContext* pDevContext )override;

    virtual TsBool UpdateMaterial( TsDeviceContext* pDevContext )override;
    virtual TsBool ApplyMaterial( TsDeviceContext* pDevContext )override;

    TsBool SetTransform( TsTransForm* pTransform );
private:
    //=========================================================================
    //! propery
    //=========================================================================
    TsVertexBuffer*         m_vertexBuffer;
    TsVertexSkin            m_vertex[4];
    TsTransformCBuffer*     m_transformCBuffer;
    TsDefaultMatrial*       m_pMaterial;
};