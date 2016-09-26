//!*******************************************************
//!	TsFileUT.h
//!
//!	String Utility.
//!
//!	©	2016 Yuki Tsuneyama
#pragma once

namespace TSUT
{
	//ファイルパスからファイル名だけを取得
	inline TsString PassToFileName( TsString str )
	{
		return str.substr( str.rfind( "\\" ) + 1 , str.size() - 1 );
	}

	// バイナリシェーダが格納されているパスを取得
	inline TsString GetCSODirectory()
	{
		return "Resource/Shader/";
	}

	//! 拡張子の取得
	inline TsString FileToExtension( TsString str )
	{
		return str.substr( str.rfind( '.' ) , str.size() - 1 );
	}

	//! 文字列をハッシュ値に変換
	inline TS_HASH StringToHash( TsString str )
	{
		TS_HASH h = 0;
		const char* val = str.c_str();
		int len = str.length();

		for( int i = 0; i < len; i++ ) {
			h = 31 * h + val[i];
		}
		return h;
	}
}
