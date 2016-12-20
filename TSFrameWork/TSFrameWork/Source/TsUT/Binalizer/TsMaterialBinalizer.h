//!*******************************************************
//! TsTransformBinalizer.h
//!
//! Transformをバイナリ化するクラス.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsDefaultMaterial;
class TsDevice;

class TsMaterialBinalizer : public TsBinalizer
{
public:
    //----------------------------------------------------------
    // Define
    //----------------------------------------------------------
#pragma pack(1)
    struct MaterialCommon
    {
        TsChar   name[128];
        TsFloat4 diffuse;     
        TsFloat3 specluar;
        TsF32    power;
        TsFloat4 ambient;     
        TsFloat3 emissive;    
        TsF32    mataric;     
        TsF32    roughness;   

        TsChar   albedoTexture[128];
        TsChar   normalTexture[128];
        TsChar   specluerTexture[128];
    };
#pragma pack()


    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------

    TsMaterialBinalizer();

    TsBool Binalize(std::ofstream& ofs, TsDefaultMaterial* pData,TsUint count);
    TsBool Decode( TsDevice* pDev, std::ifstream& ifs, TsBool readHeader = TS_FALSE);

    TsDefaultMaterial* GetMaterials()const;

    TsUint GetMaterialCount()const;

protected:
    //----------------------------------------------------------
    // peropery
    //----------------------------------------------------------
    TsDefaultMaterial* m_pMaterials;
    TsUint             m_materialNum;
};