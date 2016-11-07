#pragma once

class TsColor
{
public:
    //===================================================
    //! Constructor
    //===================================================
    TsColor(const TsFloat4& color = TsFloat4(1,1,1,1));
    TsColor(const TsInt4& intColor);
    TsColor(TsUint byteCode);
    TsColor(TsF32 r, TsF32 g, TsF32 b, TsF32 a);

    //===================================================
    //! public method
    //===================================================
    TsInt4 ToRGBAInt();
    TsUint ToByteCode();
    TsBool SetByInt(TsInt4 color);
    TsBool SetByByteCode(TsUint dw);
    TsBool IsAlpha();
    TsColor& operator = (const TsFloat4& f);
    TsColor& operator = (const TsColor&);
    
    //===================================================
    //! Ref Of rgba
    //===================================================
    TsF32& r = m_color.x;
    TsF32& g = m_color.y;
    TsF32& b = m_color.z;
    TsF32& a = m_color.w;
public:
    //===================================================
    //! public propery
    //===================================================
    TsFloat4 m_color;

public:
    //===================================================
    //! public static propery
    //===================================================
    static const TsColor White ;
    static const TsColor Black;
    static const TsColor Red;
    static const TsColor Blue;
    static const TsColor Green;
    static const TsColor Gray;

};