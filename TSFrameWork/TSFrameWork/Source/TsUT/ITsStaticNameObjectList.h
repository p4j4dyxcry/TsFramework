#pragma once

template<class T>
class ITsStaticNameObjectList
{
public:
	ITsStaticNameObjectList()
	{
		AddObject( static_cast<T*>(this) );
	}

	virtual ~ITsStaticNameObjectList()
	{
		RemoveObject( (T*) this );
	}


	static T* Find( TsString name )
	{
		TS_HASH hash = TSUT::StringToHash( name );

		for each( auto var in m_objectList )
		{
			if( hash == var->GetHashCode() )
			{
				return var;
			}
		}
		TsDebugLog( "StaticNameObjectList Fail Find Name Object List \n\t name = \"%s\"\n" , name.c_str() );
		return nullptr;
	}

	static TsBool RemoveObjectByName( TsString name )
	{
		auto object = this->Find( name );
		if( object == nullptr )
		{
			return TS_FALSE;
		}
		RemoveObject( object );
	}

private:
	static TsBool AddObject(T* object)
	{
		TsDebugLog("StaticNameObjectList Add Name Object List \n\t name = \"%s\" \n\t hash = %x\n", object->GetName().c_str(), object->GetHashCode());
		m_objectList.push_back(object);

		return TS_TRUE;
	}

	static TsBool RemoveObject(T * object)
	{
		m_objectList.remove(object);
		return TS_TRUE;
	}
private:
	static TsList<T*> m_objectList;
};

#define TS_INSTANTIATE_NAME_OBJ_LIST( type )\
	TsList<type*> ITsStaticNameObjectList<type>::m_objectList;