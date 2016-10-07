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
    //! 名前を取得
    TsString GetName()const{ return m_name; }

    //! 名前を設定
    void SetName( TsString str ){
        m_name = str;
        m_hashCode = TSUT::StringToHash( str );
    }

    //ハッシュコードの取得
    TS_HASH GetHashCode()const{ return m_hashCode; }
private:
    TsString m_name;		//名前
    TS_HASH m_hashCode;		//名前から計算したハッシュコード
};