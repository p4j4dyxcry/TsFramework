//!*******************************************************
//! TsDrawObject.h
//!
//! The class has a draw call.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsDrawObject : public TsNameObject
{
public:
    virtual TsBool Create( TsDevice* );
    virtual TsBool Draw( TsDeviceContext* ) ;

    virtual TsBool UpdateTransform( TsDeviceContext* context );
    virtual TsBool UpdateMaterial( TsDeviceContext* context );
    virtual TsBool UpdateIndexBuffer( TsDeviceContext* context ) ;
    virtual TsBool UpdateVertexBuffer( TsDeviceContext* context );

    virtual TsBool ApplyTransForm( TsDeviceContext * context );
    virtual TsBool ApplyMaterial( TsDeviceContext* context );
    virtual TsBool ApplyIndexBuffer( TsDeviceContext* context );
    virtual TsBool ApplyVertexBuffer( TsDeviceContext* context );

    virtual TsMatrix GetWorldMatrix();
    virtual TsMatrix GetLocalMatrix();
};