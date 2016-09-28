#pragma once

class TsColor
{
public:
	TsInt4 ToRGBAInt()
	{
		return TsInt4( m_color * 255.0f );
	}
	TsBool SetByInt(TsInt4 color)
	{
		m_color = ( TsFloat4(color) / 255.0f);
		return TS_TRUE;
	}
	TsBool IsAlpha()
	{
		return m_color.w <= 1.0f;
	}

	TsFloat4 m_color;
};