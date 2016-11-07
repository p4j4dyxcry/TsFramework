#include "../TsGfx.h"

TS_INSTANTIATE_NAME_OBJ_LIST( TsLight );

TsLight::TsLight()
 : m_intensity(1.0f),
 m_color(TsColor::White)
{

}
TsLight::~TsLight()
{

}
const TsColor& TsLight::GetColor()const
{
    return m_color;
}
TsBool  TsLight::SetColor(const TsColor& color)
{
    m_color = color;
    return TS_TRUE;
}

TsF32 TsLight::GetIntensity()const
{
    return m_intensity;
}
TsBool  TsLight::SetIntensity(TsF32 intensity)
{
    m_intensity = intensity;
    return TS_TRUE;
}
