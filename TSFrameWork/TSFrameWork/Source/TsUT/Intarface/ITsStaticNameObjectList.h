//!*******************************************************
//! ITsStaticNameObjectList.h
//!
//! Staticなリストを作成するインターフェース
//!
//! © 2016 Yuki Tsuneyama
#pragma once

template<class T>
class ITsStaticNameObjectList
{
public:
    //! コンストラクタ
    ITsStaticNameObjectList()
    {
        AddObject( static_cast<T*>(this) );
    }

    //! デストラクタ
    virtual ~ITsStaticNameObjectList()
    {
        RemoveObject( (T*) this );
    }
    //! 名前付きオブジェクトの作成
    //static T* CreateObject( const TsString& name )
    //{
    //    typename T* object = TsNew( T );
    //    object->SetName( name );
    //    return object;
    //}

    //! オブジェクトの名前でリストから取得する
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

    //! オブジェクトの名前でリストからオブジェクトを削除する
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

    //!リストにオブジェクトを追加する
    static TsBool AddObject(T* object)
    {
        TsDebugLog("StaticNameObjectList Add Name Object List \n\t name = \"%s\" \n\t hash = %x\n", object->GetName().c_str(), object->GetHashCode());
        m_objectList.push_back(object);

        return TS_TRUE;
    }

    //! リストからオブジェクトを破棄する
    static TsBool RemoveObject(T * object)
    {
        m_objectList.remove(object);
        return TS_TRUE;
    }
private:
    static TsList<T*> m_objectList;
};

//! インターフェースを継承したクラスのcppに追加する必要がある。
#define TS_INSTANTIATE_NAME_OBJ_LIST( type )\
    TsList<type*> ITsStaticNameObjectList<type>::m_objectList;