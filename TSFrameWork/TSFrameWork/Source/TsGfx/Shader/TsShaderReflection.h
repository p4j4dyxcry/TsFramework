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
    //----------------------------------------------------------
    // Define
    //----------------------------------------------------------
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

    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsShaderReflection(ID3D11ShaderReflection* pDxShaderReflection = nullptr);
    TsBool Analize(ID3D11ShaderReflection* pDxShaderReflection);

private:

    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    TsVector<TsCBufferReflection> m_constantBuffers;
};