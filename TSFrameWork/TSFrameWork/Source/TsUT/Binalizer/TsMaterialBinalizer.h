//!*******************************************************
//! TsTransformBinalizer.h
//!
//! Materialをバイナリ化するクラス.
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
    struct CommonMaterial
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
    virtual ~TsMaterialBinalizer();

    TsBool Binalize(std::ofstream& ofs, TsDefaultMaterial** pData,TsUint count);
    TsBool Decode(std::ifstream& ifs);

    TsBool BuildMaterial(TsDevice* pDev);

    TsDefaultMaterial* GetMaterials()const
    {
        return m_pMaterials;
    }

    TsUint GetMaterialCount()const
    {
        return m_materialNum;
    }

protected:
    //----------------------------------------------------------
    // peropery
    //----------------------------------------------------------
    TsDefaultMaterial* m_pMaterials;
    CommonMaterial*    m_pCommonMaterials;
    TsUint             m_materialNum;
};