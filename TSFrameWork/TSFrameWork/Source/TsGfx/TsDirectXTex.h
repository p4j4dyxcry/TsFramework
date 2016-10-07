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
    static ID3D11ShaderResourceView * LoadFromFile( ID3D11Device * pDev ,const TsChar* fileName );
};