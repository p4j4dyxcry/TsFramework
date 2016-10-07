//!*******************************************************
//! TsShaderResource.h
//!
//! he class is Shader Resource( Texture,CBuffer...etc) base.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsShaderResource 
{
public:
    TsShaderResource() :m_registerIndex(0){}

    TsInt GetRegisterIndex()const{ return m_registerIndex; }
    void  SetRegisterIndex(TsInt registerIndex){ m_registerIndex = registerIndex; }
protected:
    TsInt m_registerIndex;
};