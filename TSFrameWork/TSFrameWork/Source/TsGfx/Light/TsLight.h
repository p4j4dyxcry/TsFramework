#pragma once

class TsLight :
    public TsNameObject,
    public ITsStaticNameObjectList<TsLight>,
    public IHasTransform
{
public:
    enum class LIGHT_TYPE : TsUint
    {
        LIGHT_DIRECTIONAL = 0,
        LIGHT_POINT = 1,
        LIGHT_SPOT = 2,
    };

    TsColor GetColor();
    TsBool  SetColor( TsColor );
    virtual LIGHT_TYPE GetLightType() = 0;
    TsBool Enable()const{ return m_isEnable; }
    TsBool SetEnable( TsBool enable ){ m_isEnable = enable; }
protected:
    TsColor m_color;
    TsF32   m_intensity;
    TsBool  m_isEnable;
};



class TsDirectioalLight :public TsLight
{
public:
    LIGHT_TYPE GetLightType()override
    {
        return LIGHT_TYPE::LIGHT_DIRECTIONAL;
    }
    TsVector3 GetLightDir()
    {
        return TsVector3::up * m_pTransform->ToWorldMatrix();
    }
protected:

};

class TsPointLight : public TsLight
{
public:
    LIGHT_TYPE GetLightType()override
    {
        return LIGHT_TYPE::LIGHT_POINT;
    }
protected:
    TsF32 m_rage;
};

class TsSpotLight : public TsLight
{
    LIGHT_TYPE GetLightType()override
    {
        return LIGHT_TYPE::LIGHT_SPOT;
    }
protected:
    TsF32 m_range;
    TsF32 m_angle;
};