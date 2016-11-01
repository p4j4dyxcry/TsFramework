//!*******************************************************
//! TsGeometryObject.h
//!
//! Mesh base.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsGeometryObject : virtual public TsDrawObject
{
public:
    TsGeometryObject();
    virtual ~TsGeometryObject();
    virtual TsBool CreateGeometryObject( TsDevice* pDev ,
                                         TsVertexElement * pMesh ,
                                         TsMaterial* pMaterial );

    virtual TsBool UpdateTransform( TsDeviceContext* context )override;
    virtual TsBool UpdateMaterial( TsDeviceContext* context )override;
    virtual TsBool UpdateIndexBuffer( TsDeviceContext* context )override;
    virtual TsBool UpdateVertexBuffer( TsDeviceContext* context )override;

    virtual TsBool ApplyTransForm( TsDeviceContext * context )override;
    virtual TsBool ApplyMaterial( TsDeviceContext* context )override;
    virtual TsBool ApplyIndexBuffer( TsDeviceContext* context )override;
    virtual TsBool ApplyVertexBuffer( TsDeviceContext* context )override;
    virtual TsBool Draw( TsDeviceContext* )override;

    TsBool SetMaterial( TsMaterial* pMaterial );
    TsBool SetTransform( TsTransForm * pTransform );
    TsBool SetMesh( TsVertexElement* );

    TsTransForm* GetTransform()const;
private:
    TsTransForm*            m_transform;
    TsVertexElement*                 m_mesh;
    TsMaterial*             m_material;
    TsTransformCBuffer*     m_transformCBuffer;
};