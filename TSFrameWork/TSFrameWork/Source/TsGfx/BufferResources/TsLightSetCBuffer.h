//**********************************************************
//! TsLightSetCBuffer.h
//! ライトをまとめたConstantBuffer.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

class TsLightSetCBuffer : public TsCBuffer
{
public:
    //====================================================
    // Define Number & Struct
    //====================================================
    static const TsUint LIGHT_CB_MAX = 256;

    struct LightData
    {
      TsMatrix            worldToShadowMatrix;    //directional Only
      TsFloat4            color;
      TsVector4           pos;
      TsVector4           dir;

      TsUint              type;
      TsF32               intensity;
      TsF32               range;
      TsF32               angle;
    };
    struct LightCB
    {
        LightData           lightData[LIGHT_CB_MAX];
        TsUint              lightNum;
        TsF32               dumy[3];
    };

    TsBool AddLight(TsLight * pLight);
    TsBool RemoveLight(TsLight *pLight);
    TsBool RemoveLightByName(const TsString& name);

public:
    //====================================================
    //  ! public methods
    //====================================================
    TsLightSetCBuffer();
    virtual ~TsLightSetCBuffer();

    TsBool CreateLightSetCBuffer( TsDevice* pDev );
    virtual TsBool ApplyCBuffer( TsDeviceContext* pDevContext )override;
    virtual TsBool UpdateCBuffer( TsDeviceContext* pDevContext )override;
protected:
    LightCB m_lightSetCBuffer;              //  Constant Buffer
    TsVector<TsLight*>  m_pLightRefList;    //  GPUに転送するライト
    TsBool m_isUpdate;
};