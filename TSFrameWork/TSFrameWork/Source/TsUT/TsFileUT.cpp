#include "TsUT.h"
#include "TsFileUT.h"
#include "../TsOS/TsWindowsUtility.h"

#include <sstream>

using namespace TSUT;

//----------------------------------------------------------
//! Constructor
//----------------------------------------------------------
TsFilePathAnalyzer::TsFilePathAnalyzer( TsString filepath )
{
    Analize( filepath );
}

//----------------------------------------------------------
//! Analize File Path
//----------------------------------------------------------
TsBool TsFilePathAnalyzer::Analize( TsString filepath )
{
    m_filePath = filepath;
    m_extension = FileToExtension( m_filePath );
    m_fullPath = LocalToFullPath( m_filePath );
    m_fileName = PassToFileName( m_filePath );
    m_localDirectory = FileToLocalDirectory( m_filePath );
    m_fullDirectory = LocalToFullPath( m_localDirectory );

    return TS_TRUE;
}

//----------------------------------------------------------
//! Get Loaded File Path
//----------------------------------------------------------
TsString TsFilePathAnalyzer::GetFilePath()
{
    return m_filePath;
}

//----------------------------------------------------------
//! Get Full Dirctory Path
//----------------------------------------------------------
TsString TsFilePathAnalyzer::GetFullPath()
{
    return m_fullPath;
}

//----------------------------------------------------------
//! Get Extension
//----------------------------------------------------------
TsString TsFilePathAnalyzer::GetExtension()
{
    return m_extension;
}

//----------------------------------------------------------
//! Get Local File Path Sub Extension
//----------------------------------------------------------
TsString TsFilePathAnalyzer::GetFileName()
{
    return m_fileName;
}

//----------------------------------------------------------
//! Get Local File Directory Path
//----------------------------------------------------------
TsString TsFilePathAnalyzer::GetLocalDirectory()
{
    return m_localDirectory;
}

//----------------------------------------------------------
//! Get Full Directory Path
//----------------------------------------------------------
TsString TsFilePathAnalyzer::GetFullDirectory()
{
    return m_fullDirectory;
}


//! 名前を変更する
TsBool TsFilePathAnalyzer::ReName(const TsString& newName)
{
    return Analize(newName);
}

//! 拡張子を変更する
TsBool TsFilePathAnalyzer::ReExtencion(const TsString& extencion)
{
    auto index = m_fullPath.rfind('.');
    return Analize(m_fullPath.substr(0, index) + extencion);
}

TsFileDirectory::TsFileDirectory( TsString directory ):
m_searchSubFolder(TS_TRUE)
{
    if( directory.empty() )
        return;
    m_directory = directory;
    if( m_directory[m_directory.size() - 1] != '/' &&
        m_directory[m_directory.size() - 1] != '\\' )
        m_directory += '/';
}

void TsFileDirectory::SetSearchSubFolderFlag( TsBool searchSubFolder )
{
    m_searchSubFolder = searchSubFolder;
}
TsBool TsFileDirectory::AddExtension( TsString& extencion )
{
    m_extensions.push_back( extencion );
    return TS_TRUE;
}
TsBool TsFileDirectory::RemoveExtension( TsString& extencion )
{
    m_extensions.remove( extencion );
    return TS_TRUE;
}
TsList<TsString> TsFileDirectory::GetFileList()
{
    TsList<TsString> fileList;
    WIN32_FIND_DATA tFindFileData;
    // 最初に一致するファイルを取得
    HANDLE hFile = ::FindFirstFile( (m_directory + "*").c_str() , &tFindFileData );

    if( INVALID_HANDLE_VALUE == hFile ) {
        return fileList;
    }
    do {

        TsString filename = tFindFileData.cFileName;

        //! 自身を参照すると無限ループになるので回避
        if( filename == "." || filename== ".." )
            continue;

        // フォルダかどうかを判定
        if( tFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) 
        {
            //サブフォルダの検索
            if( m_searchSubFolder == TS_FALSE)
                continue;

            TsFileDirectory subfolder( m_directory + filename  );
            subfolder.SetSearchSubFolderFlag( TS_TRUE );
            for each ( auto var in m_extensions )
                subfolder.AddExtension( var );

            TsList<TsString>&& subFolderFiles = subfolder.GetFileList();
            for each ( auto var in subFolderFiles )
                fileList.push_back( var );
        }
        else 
        {
            TsString extention = TSUT::FileToExtension( filename );

            // 拡張子が設定されていない場合は全てのファイルを格納
            if( m_extensions.empty() )
                fileList.push_back( m_directory + filename );
            else
            {
                //拡張子が設定されている場合は指定の拡張子のファイルならリストに追加
                for each ( auto var in m_extensions )
                    if( var == extention )
                    {
                        fileList.push_back( m_directory + filename );
                        break;
                    }
            }
        }

        // 次に一致するファイルの検索
    } while( ::FindNextFile( hFile , &tFindFileData ) );

    // 検索ハンドルを閉じる
    ::FindClose( hFile );
    return fileList;
}

TsString TSUT::LocalToFullPath( TsString localPath )
{
    char szFullPath[MAX_PATH] = { '\0' };
    char *szFilePart;

    GetFullPathName(
        localPath.c_str() , /* ファイル名を相対パスで指定 */
        sizeof( szFullPath ) / sizeof( szFullPath[0] ) ,
        szFullPath ,
        &szFilePart );

    return szFullPath;
}

TsString TSUT::IntToString(TsInt data)
{
    std::stringstream ss;
    ss << data;
    return ss.str();
}
TsString TSUT::UintToString(TsUint data)
{
    std::stringstream ss;
    ss << data;
    return ss.str();
}
TsString TSUT::U64ToString(TsU64 data)
{
    std::stringstream ss;
    ss << data;
    return ss.str();
}
TsString TSUT::F32ToString(TsF32 data)
{
    std::stringstream ss;
    ss << data;
    return ss.str();
}
TsString TSUT::F64ToString(TsF64 data)
{
    std::stringstream ss;
    ss << data;
    return ss.str();
}