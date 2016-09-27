#pragma once

class TsLight :
	public TsNameObject ,
	public ITsStaticNameObjectList<TsLight>
{
public:
	TsLight()
	{
		m_transform = TsNew( TsTransForm );
	}
	virtual ~TsLight()
	{
		TsSafeDelete( m_transform );
	}
	TsBool SetRotate( TsQuaternion& q )
	{
		m_transform->m_localRotate = q;
	}
	TsTransForm GetTransform()const
	{
		return *m_transform;
	}

	TsVector3 GetDir()const
	{
		return TsVector3::up * m_transform->ToWorldMatrix();
	};

protected:
	TsTransForm* m_transform;
};