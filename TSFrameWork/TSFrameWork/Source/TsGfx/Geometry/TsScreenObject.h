//**********************************************************
//! TsScreenObject.h
//! PostProcess実行用のスクリーンを覆うクラスです.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

class TsScreenObject : public TsDrawObject
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsScreenObject();
    virtual ~TsScreenObject();

    virtual TsBool Create( TsDevice* pDev ) override;
    virtual TsBool Draw( TsDeviceContext* context ) override;
    virtual TsBool ApplyVertexBuffer( TsDeviceContext* context )override;
    virtual TsBool ApplyIndexBuffer( TsDeviceContext* context )override;
    virtual TsBool ApplyTransForm( TsDeviceContext* context )override;
protected:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    TsVertexBuffer*         m_vertexBuffer;
    TSVertexScreenSpace     m_vertex[4];
};