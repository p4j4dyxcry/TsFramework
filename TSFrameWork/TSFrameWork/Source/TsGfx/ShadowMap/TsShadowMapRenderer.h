//**********************************************************
//! TsShadoMapRenderer.h
//! todo.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

class TsShadowMapRenderer 
{
public:
    TsLight* m_pLight;
protected:
    TsShaderEffect* m_pShader;
    static TsRenderTarget* m_pShadewmapTexture;
};