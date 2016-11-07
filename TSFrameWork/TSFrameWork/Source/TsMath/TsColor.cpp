#include "../../TsAfx.h"

const TsColor TsColor::White    = TsColor(1, 1, 1, 1);
const TsColor TsColor::Black    = TsColor(0, 0, 0, 1);
const TsColor TsColor::Red      = TsColor(1, 0, 0, 1);
const TsColor TsColor::Blue     = TsColor(0, 1, 0, 1);
const TsColor TsColor::Green    = TsColor(0, 0, 1, 1);
const TsColor TsColor::Gray     = TsColor(.5, .5, .5, 1);

TsColor::TsColor(const TsFloat4& color )
{
    *this = color;
}

TsColor::TsColor(const TsInt4& intColor)
{
    SetByInt(intColor);
}

TsColor::TsColor(TsUint byteCode)
{
    SetByByteCode(byteCode);
}

TsColor::TsColor(TsF32 r, TsF32 g, TsF32 b, TsF32 a)
{
    m_color = TsFloat4(r, g, b, a);
}


TsInt4 TsColor::ToRGBAInt()
{
    return TsInt4(m_color * 255.0f);
}

TsUint TsColor::ToByteCode()
{
    TsInt4 &&rgba = ToRGBAInt();
    TsUint result = 0;
    result |= (rgba.w << 6);
    result |= (rgba.x << 4);
    result |= (rgba.y << 2);
    result |= (rgba.z);
    return result ;
}
TsBool TsColor::SetByInt(TsInt4 color)
{
    m_color = (TsFloat4(color) / 255.0f);
    return TS_TRUE;
}
TsBool TsColor::SetByByteCode(TsUint dw)
{
    TsInt4 rgba;
    rgba.w = (dw & 0xff000000) >> 6;
    rgba.x = (dw & 0x00ff0000) >> 4;
    rgba.y = (dw & 0x0000ff00) >> 2;
    rgba.z = (dw & 0x000000ff);
    return SetByInt(rgba);
}

TsBool TsColor::IsAlpha()
{
    return m_color.w <= 1.0f;
}

TsColor& TsColor::operator = (const TsFloat4& f)
{
    m_color = f;
    return *this;
}

TsColor& TsColor::operator = (const TsColor& f)
{
    m_color = f.m_color;
    return *this;
}
