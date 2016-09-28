#pragma once

class TsLight :
	public TsNameObject,
	public ITsStaticNameObjectList<TsLight>,
	public IHasTransform
{
public:
	enum LIGHT_TYPE
	{
		LIGHT_DIRECTIONAL,
		LIGHT_POINT,
		LIGHT_SPOT,
	};

	TsColor GetColor();
	TsBool  SetColor( TsColor );
	virtual LIGHT_TYPE GetLightType() = 0;
protected:
	TsColor m_color;
	TsF32   m_intensity;
};

class TsDirectioalLight :public TsLight
{
public:
	LIGHT_TYPE GetLightType()override
	{
		return LIGHT_DIRECTIONAL;
	}
	TsVector3 GetLightDir()
	{
		return TsVector3::up * m_pTransform->ToWorldMatrix();
	}
};