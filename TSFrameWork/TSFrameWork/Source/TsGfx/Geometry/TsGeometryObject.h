//!*******************************************************
//! TsGeometryObject.h
//!
//! this class has Vertex Element , Material And Material.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsGeometryObject : virtual public TsDrawObject
{
public:
    //! Constructor
    TsGeometryObject();

    //! Destructor
    virtual ~TsGeometryObject();


    //! CreateGeometry
    //
    // @param in pDev       Device.
    // @param in pMesh      Vertex Elements.
    // @param in pMaterial  Material
    virtual TsBool CreateGeometryObject( TsDevice* pDev ,
                                         TsVertexElement * pVertexElement ,
                                         TsMaterial* pMaterial );

    //! public Override Methods
    virtual TsBool UpdateTransform( TsDeviceContext* context )override;
    virtual TsBool UpdateMaterial( TsDeviceContext* context )override;
    virtual TsBool UpdateIndexBuffer( TsDeviceContext* context )override;
    virtual TsBool UpdateVertexBuffer( TsDeviceContext* context )override;

    virtual TsBool ApplyTransForm( TsDeviceContext * context )override;
    virtual TsBool ApplyMaterial( TsDeviceContext* context )override;
    virtual TsBool ApplyIndexBuffer( TsDeviceContext* context )override;
    virtual TsBool ApplyVertexBuffer( TsDeviceContext* context )override;
    virtual TsBool Draw( TsDeviceContext* )override;

    //! Set Method
    TsBool SetMaterial( TsMaterial* pMaterial );
    TsBool SetTransform( TsTransForm * pTransform );
    TsBool SetMesh( TsVertexElement* );

    //! Get Method
    TsTransForm* GetTransform()const;

    virtual TsMatrix GetWorldMatrix()const;
    virtual TsMatrix GetLocalMatrix()const;
    TsVertexElement* GetVertexElement()const
    {
        return m_mesh;
    }
    TsMaterial* GetMaterial()const
    {
        return m_material;
    }
private:

    //! propery
    TsTransForm*            m_transform;
    TsVertexElement*        m_mesh;
    TsMaterial*             m_material;
    TsTransformCBuffer*     m_transformCBuffer;
};