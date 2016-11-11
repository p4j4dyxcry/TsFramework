//**********************************************************
//! TsLight.h
//! Light Class.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

//----------------------------------------------------------
//! Light Base
//----------------------------------------------------------
class TsLight :
    public TsNameObject,
    public ITsStaticNameObjectList<TsLight>,
    public IHasTransform
{
public:
    //----------------------------------------------------------
    // Define
    //----------------------------------------------------------
    enum class LIGHT_TYPE : TsUint
    {
        LIGHT_DIRECTIONAL = 0,
        LIGHT_POINT = 1,
        LIGHT_SPOT = 2,
    };

    TsLight();
    virtual ~TsLight();

    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    const TsColor& GetColor()const;
    TsBool  SetColor(const TsColor& );

    TsF32  GetIntensity()const;
    TsBool SetIntensity(TsF32);

    virtual LIGHT_TYPE GetLightType() = 0;
    TsBool Enable()const{ return m_isEnable; }
    TsBool IsUpdate()const{ return m_isUpdate; }
    TsBool SetEnable( TsBool enable ){ m_isEnable = enable; }
protected:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    TsColor m_color;
    TsF32   m_intensity;
    TsBool  m_isEnable;
    TsBool  m_isUpdate;
};


//----------------------------------------------------------
//! Directional Light
//----------------------------------------------------------
class TsDirectioalLight :public TsLight
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    LIGHT_TYPE GetLightType()override
    {
        return LIGHT_TYPE::LIGHT_DIRECTIONAL;
    }
    TsVector3 GetLightDir()
    {
        TsVector3 v = TsVector3::front * m_pTransform->GetWorldRotate().ToMatrix();
        return v.Normalized();
    }
protected:

};

//----------------------------------------------------------
//! Point Light
//----------------------------------------------------------
class TsPointLight : public TsLight
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    LIGHT_TYPE GetLightType()override
    {
        return LIGHT_TYPE::LIGHT_POINT;
    }
protected:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    TsF32 m_rage;
};

//----------------------------------------------------------
//! Spot Light
//----------------------------------------------------------
class TsSpotLight : public TsLight
{
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    LIGHT_TYPE GetLightType()override
    {
        return LIGHT_TYPE::LIGHT_SPOT;
    }
protected:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    TsF32 m_range;
    TsF32 m_angle;
};