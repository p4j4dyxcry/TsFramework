//**********************************************************
//! TsLambertMaterial.h
//! lambert material.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsDefaultMatrial : public TsMaterial
{
public:
    //=========================================================================
    //! Define 
    //=========================================================================
    static const TsUint AlbedoTextureRegister  = 8;
    static const TsUint NormalTextureRegister  = 9;
    static const TsUint SpecuerTextureRegister = 10;

    struct DefaultMatrial
    {
        TsFloat4 diffuse;       // xyz = diffuse w = alphaTest
        TsFloat4 specluar;      // xyz = color w = power
        TsFloat4 ambient;       // xyz = color w = not using
        TsFloat3 emissive;      // xyz = emissive w
        TsF32    mataric;       // mataric
        TsF32    roughness;     // roughness

        TsBool   useDiffuseMap; // true on using albedo map
        TsBool   useNomalMap;   // true on using nomal map
        TsBool   useSpeculerMap;// true on using speclur map
        DefaultMatrial()
        {
            diffuse = TsFloat4( 1 , 1 , 1 , 1 );
            specluar = TsFloat4( 1 , 1 , 1 , 0 );
            ambient = TsFloat4( .5 , .5 , .5 , .5 );
            emissive = TsFloat3( 0 , 0 , 0 );
            mataric = 0;
            roughness = 0;

            useDiffuseMap = TS_FALSE;
            useNomalMap = TS_FALSE;
            useSpeculerMap = TS_FALSE;
        }
    };
    //=========================================================================
    //! public method
    //=========================================================================
    TsDefaultMatrial();
    virtual ~TsDefaultMatrial();

    virtual TsBool ApplyMaterial( TsDeviceContext* ) override;
    virtual TsBool CreateMaterial( TsDevice* ) override;
    virtual TsBool UpdateMaterial( TsDeviceContext* )override;

    TsBool LoadAlbedoTextureFromFile( TsDevice* pDev , const TsString& file );
    TsBool LoadNormalTextureFromFile( TsDevice* pDev , const TsString& file );
    TsBool LoadSpeculerTextureFromFile( TsDevice* pDev , const TsString& file );

    TsBool SetAlbedoTexture( TsTexture* pTex );
    TsBool SetNormalTexture( TsTexture* pTex );
    TsBool SetSpeculerTexture( TsTexture* pTex );

    TsBool SetColor( const TsFloat4& color );
    TsBool SetAmbient( const TsFloat4& color );
    TsBool SetEmissive( const TsFloat3& color );
    TsBool SetSpeculer( const TsFloat3& color );
    TsBool SetSpeculerPower( TsF32 );
    TsBool SetMataric( TsF32 mataric );
    TsBool SetRoughness( TsF32 roughness );

    TsTexture* GetAlbedoTexture()const;
    TsTexture* GetNormalTexture()const;
    TsTexture* GetSpeculerTexture()const;

    TsFloat4 GetColor()const;
    TsFloat4 GetSetAmbient()const;
    TsFloat3 GetEmissive()const;
    TsFloat3 Getpeculer()const;
    TsF32 GetSpeculerPower()const;
    TsF32 GetMataric()const;
    TsF32 GetRoughness()const;

protected:
    //=========================================================================
    //! private method
    //=========================================================================
    TsTexture * LoadTextureFromFile( TsDevice * , const TsString& );
    //=========================================================================
    //! propery
    //=========================================================================

    DefaultMatrial m_material;

    TsTexture* m_pAlbedoTexture;
    TsTexture* m_pNormalTexture;
    TsTexture* m_pSpeclurTexture;
};
