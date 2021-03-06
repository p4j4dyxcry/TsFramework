﻿//**********************************************************
//! TsSkyBox.h
//! CubeMapをレンダリングするための内側を向いた立方体を定義.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsSkyBox : public TsDrawObject
{
public:
    TsSkyBox();
    virtual ~TsSkyBox();

    virtual TsBool Create(TsDevice* pDev) override;
    virtual TsBool Draw(TsDeviceContext* context) override;
    virtual TsBool UpdateTransform(TsDeviceContext* context)override;
    virtual TsBool ApplyVertexBuffer(TsDeviceContext* context)override;
    virtual TsBool ApplyIndexBuffer(TsDeviceContext* context)override;
    virtual TsBool ApplyTransForm(TsDeviceContext* context)override;
    virtual TsBool ApplyMaterial(TsDeviceContext* context)override;
    TsBool SetCubeMap(TsCubeMap* pCubeMap);
protected:
    TsVertexBuffer*         m_pVertexBuffer;
    TsIndexBuffer*          m_pIndexBuffer;
    TSVertexSkyBox          m_vertex[6 * 4];

    TsMatrix                m_matrixCatch;
    TsTransformCBuffer*     m_pTransformCBuffer;
    TsTransForm*            m_pTransform;

    TsCubeMap*              m_pCubeMap;
};