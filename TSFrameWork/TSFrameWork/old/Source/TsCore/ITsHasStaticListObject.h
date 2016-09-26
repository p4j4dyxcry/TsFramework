#pragma once

#include "../TsUT/TsUT.h"

template <class T>
class ITsHasStaticObject
{
public:

	virtual ~ITsHasStaticObject()
	{
		this->RemoveObject((T*)this);
	}

	static const T* FindObject(TsString name)
	{
		TS_HASH hash = TSUT::StringToHash(name);

		for each(auto var in m_objectList)
		{
			if (hash == var->GetHashCode())
			{
				return var;
			}
		}
		return nullptr;
	}
	static TsBool AddObject(T* object)
	{
		TsDebugLog("Add Objcet List \n\t name = \"%s\" \n\t hash = %x\n", object->GetName().c_str(), object->GetHashCode());
		m_objectList.push_back(object);

		return TS_TRUE;
	}
	static TsBool RemoveObjectByName(TsString name)
	{
		auto object = this->FindObject(name);
		if (object == nullptr)
		{
			TsDebugLog("Faild Remove Object List \"%s\" = none \n");
			return TS_FALSE;
		}
		RemoveObject(object);
		return TS_TRUE;
	}
	TsBool RemoveObject(T* object)
	{
		m_objectList.remove(object);
		return TS_TRUE;
	}
protected: 
	static TsList<T*> m_objectList;
};

#define TS_STATIC_LIST_INSTANTIATE( type ) TsList<type*> ITsHasStaticObject<type>::m_objectList;