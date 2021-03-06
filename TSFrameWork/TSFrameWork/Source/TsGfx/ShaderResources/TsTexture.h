﻿//!*******************************************************
//! TsTexture.h
//!
//! Dx11 Texture Lapping.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsTexture : public TsShaderResource , 
                  public TsNameObject 
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsTexture() :m_srv( nullptr ){};
    virtual ~TsTexture()
    {
        TsSafeRelease( m_srv );
    }
    ID3D11ShaderResourceView * GetSRV()const {return m_srv;}
    TsBool SetSRV( ID3D11ShaderResourceView*srv )
    {
        if( m_srv )
            return TS_FALSE;
        m_srv = srv; 
        return TS_TRUE;
    }
    TsBool IsAlpha()const
    {
        return m_isAlpha;
    }
    TsBool SetAlphaMode( TsBool flag)
    {
        m_isAlpha = flag;
        return TS_TRUE;
    }
protected:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    ID3D11ShaderResourceView * m_srv;
    TsInt m_registerIndex = 0;
    TsBool m_isAlpha = TS_FALSE;
};

class TsTexture2D : public TsTexture
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsTexture2D() :
        TsTexture(), 
        m_tex2d( nullptr ){};
    TsBool CreateTextureFromBinary( TsDevice* pDevice,
                                    void* pBinary,
                                    size_t sz,
                                    DXGI_FORMAT format,
                                    TsUint width,
                                    TsUint height);
    virtual ~TsTexture2D()
    {
        TsSafeRelease( m_tex2d );
    }

protected:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    ID3D11Texture2D * m_tex2d;
};

class TsCubeMap :public TsTexture
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsCubeMap() :TsTexture()
    {
        ZeroMemory(m_tex2d, sizeof(m_tex2d));
    };
    virtual ~TsCubeMap()
    {
        for ( auto p : m_tex2d )
            TsSafeRelease(p);
    }
protected:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    ID3D11Texture2D * m_tex2d[6];
};

class TsTexture3D : public TsTexture
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsTexture3D() :TsTexture(),m_tex3d(nullptr){}
    virtual ~TsTexture3D()
    {
        TsSafeRelease(m_tex3d);
    };
protected:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    ID3D11Texture3D * m_tex3d;
};

class TsTexture1D : public TsTexture
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsTexture1D() :TsTexture(), m_tex1d(nullptr){}
    virtual ~TsTexture1D()
    {
        TsSafeRelease(m_tex1d);
    };
protected:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    ID3D11Texture1D * m_tex1d;
};
