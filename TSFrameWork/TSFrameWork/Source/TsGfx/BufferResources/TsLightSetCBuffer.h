#pragma once

class TsLightSetCBuffer : public TsCBuffer
{
public:
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
        LightData           lightData[ LIGHT_CB_MAX ];
    };
};