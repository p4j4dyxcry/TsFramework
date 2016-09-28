#pragma once

class TsLight :
	public TsNameObject ,
	public ITsStaticNameObjectList<TsLight>,
	public IHasTransform
{
public:
	TsVector3 GetDir()const
	{
		return TsVector3::up * m_pTransform->ToWorldMatrix();
	};
};