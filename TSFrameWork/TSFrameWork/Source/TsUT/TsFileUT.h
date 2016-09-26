//!*******************************************************
//!	TsFileUT.h
//!
//!	String Utility.
//!
//!	©	2016 Yuki Tsuneyama
#pragma once

namespace TSUT
{
	class TsFilePathAnalyzer
	{
	public:
		TsFilePathAnalyzer( TsString filepass );

		//ファイルパスを解析する
		TsBool Analize( TsString str );

		//! ファイル名を取得する
		TsString GetFilePath();

		//! ファイルのフルパスを取得する
		TsString GetFullPath();

		//! ファイルの拡張子を取得する
		TsString GetExtension();

		//! 拡張子を除いたファイル名を取得する
		TsString GetFileName();

		//! ローカルディレクトリを取得する
		TsString GetLocalDirectory();

		//! フルディレクトリを取得する
		TsString GetFullDirectory();
	protected:
		TsString m_filePath;
		TsString m_fullPath;
		TsString m_extension;
		TsString m_fileName;
		TsString m_localDirectory;
		TsString m_fullDirectory;
	};

	class TsFileDirectory
	{
	public:
		//! Constructor
		TsFileDirectory( TsString directory );

		//! サブフォルダを検索対象に含める？
		// TRUE		含める
		// FALSE	含めない
		void SetSearchSubFolderFlag( TsBool searchSubFolder );

		//! 検索する拡張子を追加
		TsBool AddExtension( TsString& extencion );

		//! 検索する拡張子を削除
		TsBool RemoveExtension( TsString& extencion );

		//! すべてのファイルパスを取得する
		TsList<TsString> GetFileList();
	protected:
		TsString m_directory;
		TsList<TsString> m_extensions;
		TsBool m_searchSubFolder;
	};


	//ファイルパスからファイル名だけを取得
	inline TsString PassToFileName( TsString str )
	{
		auto index = str.rfind( "/" );

		if( index == TsString::npos )
			index = str.rfind( "\\" );

		if( index == TsString::npos )
			return str;

		return str.substr( index + 1, str.size() - 1 );
	}
	namespace Resource
	{
		inline TsString GetResourceDirectory()
		{
			return "Resource/";
		}

		// バイナリシェーダが格納されているパスを取得
		inline TsString GetCSODirectory()
		{
			return "Resource/Shader/";
		}

		// シェーダパスが格納されているパスを取得
		inline TsString GetShaderPassDirectory()
		{
			return "Resource/ShaderPass/";
		}


		// シェーダパスが格納されているパスを取得
		inline TsString GetShaderFlowDirectory()
		{
			return "Resource/ShaderFlow/";
		}

		inline TsString GetShaderResourceDirectory()
		{
			return "Resource/ShaderResource/";
		}
	}


	//! 拡張子の取得
	inline TsString FileToExtension( TsString str )
	{
		auto index = str.rfind( '.' );
		if( index == TsString::npos )
			return "";
		return str.substr( str.rfind( '.' ) , str.size() - 1 );
	}

	//! フルパスの取得
	TsString LocalToFullPath( TsString localPath );

	inline TsString FileToLocalDirectory( TsString str )
	{
		auto index = str.rfind( "/" );

		if( index == TsString::npos )
			index = str.rfind( "\\" );

		if( index == TsString::npos )
			return str;

		return str.substr( 0 , index+1 );
	}

	//! 文字列をハッシュ値に変換
	inline TS_HASH StringToHash(const TsString& str )
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
