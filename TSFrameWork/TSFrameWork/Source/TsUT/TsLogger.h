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

    enum class TS_CONSOLE_COLOR
    {
        BLACK       =  0x00 ,
        DARK_BLUE   =  0x01 ,
        DARK_GREEN  =  0x02 ,
        DARK_CYAN   =  0x03 ,
        DARK_RED    =  0x04 ,
        DARK_VIOLET =  0x05 ,
        DARK_YELLOW =  0x06 ,
        GRAY        =  0x07 ,
        LIGHT_GRAY  =  0x08 ,
        BLUE        =  0x09 ,
        GREEN       =  0x0a ,
        CYAN        =  0x0b ,
        RED         =  0x0c ,
        VIOLET      =  0x0d ,
        YELLOW      =  0x0e ,
        WHITE       =  0x0f 
    };
    void TsConsoleColor(TS_CONSOLE_COLOR);
};