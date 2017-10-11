//!*******************************************************
//! TsNameObject.h
//!
//! 名前付きオブジェクトの基底クラス
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsNameObject
{
public:
    virtual ~TsNameObject() = default;
    //! 名前を取得
    virtual TsString GetName()const{ return m_name; }

    //! 名前を設定
    virtual void SetName( TsString str ){
        m_name = str;
        m_hashCode = TSUT::StringToHash( str );
    }

    virtual TsString ClassName()
    {
        TsString& str = TsString( typeid( *this ).name() );
        str = str.substr( str.rfind( " " )+1 );
        return str;
    }

    //ハッシュコードの取得
    TS_HASH GetHashCode()const{ return m_hashCode; }
private:
    TsString m_name;		//名前
    TS_HASH m_hashCode;		//名前から計算したハッシュコード
};