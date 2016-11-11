//**********************************************************
//! TsMaterial.h
//! material base.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsMaterial : public TsCBuffer
{
public:
    //----------------------------------------------------------
    // Define
    //----------------------------------------------------------
    static const TsUint AlbedoTextureRegister = 8;

    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsMaterial();
    virtual ~TsMaterial();
    virtual TsBool ApplyMaterial( TsDeviceContext* context ) ;
    virtual TsBool UpdateMaterial( TsDeviceContext* context );
    virtual TsBool CreateMaterial( TsDevice* pDevice );

    virtual TsBool LoadTextureFromFile( TsDevice* pDevice );
    virtual TsTexture* GetTexture()const;

public:
    TsString m_textureName;

protected:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    TsBool m_isUpdate;
    TsTexture*   m_pTexture;
};