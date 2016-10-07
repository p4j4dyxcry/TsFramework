//!*******************************************************
//! TsLogger.h
//!
//! TsFramework Logger
//!
//! © 2016 Yuki Tsuneyama

#pragma once

namespace TSUT
{
    //=================================================
    // TsLoggerInit
    // TsFrameworkLogSystem Initialize.
    // @pram showConsole	コンソールの出力を有効にする
    //		 writeLog		ログファイルの出力を有効にする
    //		 outputFileName	ログファイル名
    //		 outputDir		ログファイルの出力ディレクトリ(相対)
    TsBool TsLoggerInit( TsBool showConsole = TS_TRUE,
                         TsBool writeLog	= TS_TRUE,
                         TsString outputFileName = "Debug",
                         TsString outputDir = "" );

    //=================================================
    // TsLog
    // TsLoggerInit()の引数に準じた出力先にログを出力します。
    // TsLoggerInit()がコールされていない場合は何も出力しません。
    // @pram	printf()と同じ形式
    void TsLog( const char * fmt , ... );
};