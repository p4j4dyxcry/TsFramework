//!*******************************************************
//! TsViewport.h
//!
//! view port.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsViewport
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    void Create( const TsInt width ,
                 const TsInt height ,
                 const TsInt _topLeftX = 0 ,
                 const TsInt _topLeftY = 0 ,
                 const TsF32 _minDpeth = 0.0f,
                 const TsF32 _maxDpeth = 1.0f );
    TsInt2 GetSize();
    const D3D11_VIEWPORT* GetD3DViewport()const;
protected:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    D3D11_VIEWPORT	m_d3dviewport;
};