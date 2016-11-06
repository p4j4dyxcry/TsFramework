//!*******************************************************
//! TsDirectXTex.h
//!
//! Create Texture From File.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsDirectXTex
{
public:
    struct Result
    {
        ID3D11ShaderResourceView* pSrv;
        TsBool                    IsAlphaEnable;
    };
        
    static Result LoadFromFile( ID3D11Device * pDev , const TsChar* fileName );
};