#pragma once

template<class T>
class TsSingleton
{
public:
	static T GetInstance()
	{
		return m_instance;
	}
	virtual ~TsSingleton();
private:
	static T m_instance;
	TsSingleton();
};