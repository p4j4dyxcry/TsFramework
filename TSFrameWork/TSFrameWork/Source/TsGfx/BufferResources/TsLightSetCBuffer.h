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
        TsLight::LIGHT_TYPE type;
        TsColor             color;
        TsVector3           pos;
        TsVector3           dir;
        TsF32               intensity;
        TsF32               range;
        TsF32               angle;
        TsMatrix            worldToShadowMatrix;    //directional Only
    };
    struct LightCB
    {
        TsUint              lightNum;
        LightData           lightData[LIGHT_CB_MAX];
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
    TsVector<TsLight*>  m_pLightRefList;    //  GPU‚É“]‘—‚·‚éƒ‰ƒCƒg
    TsBool m_isUpdate;
};