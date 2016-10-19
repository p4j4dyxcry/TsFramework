//!*******************************************************
//! TsRenderSystem.h
//!
//! Render System
//!
//! © 2016 Yuki Tsuneyama

#pragma once

class TsShaderReflection : public TsNameObject
{
public:
    TsShaderReflection(ID3D11ShaderReflection* pDxShaderReflection = nullptr);
    TsBool Analize(ID3D11ShaderReflection* pDxShaderReflection);

    class TsCBufferReflection : TsNameObject
    {
    public:
        TsBool Analize(ID3D11ShaderReflectionConstantBuffer* 
                       pDxCbufferRefrection);
        TsBool  SetRegsiterID(TsInt id);
    private:
        TsInt       m_registerID;
        TsInt       m_offset;
    };

private:
    TsVector<TsCBufferReflection> m_constantBuffers;
};