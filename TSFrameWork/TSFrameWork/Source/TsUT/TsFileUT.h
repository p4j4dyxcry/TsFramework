//!*******************************************************
//!	TsFileUT.h
//!
//!	String Utility.
//!
//!	©	2016 Yuki Tsuneyama
#pragma once

namespace TSUT
{
	//! ファイルパスを解析するUtility
	class TsFilePathAnalyzer
	{
	public:
		TsFilePathAnalyzer(){};

		//! コンストラクタ
		TsFilePathAnalyzer( TsString filepass );

		//! ファイルパスを解析する
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

        //! 名前を変更する
        TsBool ReName(const TsString& newName);

        //! 拡張子を変更する
        TsBool ReExtencion( const TsString& extencion );
	protected:
		TsString m_filePath;		//ローカルのファイルパス
		TsString m_fullPath;		//フルパス
		TsString m_extension;		//ファイルの拡張子
		TsString m_fileName;		//ディレクトリを含まないファイルの名前
		TsString m_localDirectory;	//ファイルを含まないディレクトリ構成
		TsString m_fullDirectory;	//ファイルを含まないフルパスのディレクトリ構成
	};

	class TsFileDirectory
	{
	public:
		//! Constructor
		TsFileDirectory( TsString directory );

		//! サブフォルダを検索対象に含めるかどうかの設定
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


	//! ファイルパスからファイル名だけを取得
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
		//! リソースファイルが格納されているパスを取得
		inline TsString GetResourceDirectory()
		{
			return "Resource/";
		}

		//! RenderSystemが格納されているパスを取得
		inline TsString GetRenderSystemDirectory()
		{
			return GetResourceDirectory() + "RenderSystem/";
		}

		//! バイナリシェーダが格納されているパスを取得
		inline TsString GetCSODirectory()
		{
			return GetResourceDirectory()+"Shader/";
		}

		//! シェーダパスが格納されているパスを取得
		inline TsString GetShaderPackageDirectory()
		{
			return GetResourceDirectory()+"ShaderPackage/";
		}


		//! シェーダフローが格納されているパスを取得
		inline TsString GetShaderFlowDirectory()
		{
			return GetResourceDirectory() +"ShaderFlow/";
		}

		//! シェーダリソースが格納されているパスを取得
		inline TsString GetShaderResourceDirectory()
		{
			return GetResourceDirectory()+"ShaderResource/";
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
			return "";

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

    // 大文字へ変換する
    inline TsString StringToUpper(const TsString& str)
    {
        TsString result;
        for (auto c : str)
            result += toupper(c);
        return result;
    }

    // 小文字へ変換する
    inline TsString StringToLower(const TsString& str)
    {
        TsString result;
        for (auto c : str)
            result += tolower(c);
        return result;
    }


    TsString IntToString(TsInt);

    TsString UintToString(TsUint);

    TsString U64ToString(TsU64);

    TsString F32ToString(TsF32);

    TsString F64ToString(TsF64);
}
