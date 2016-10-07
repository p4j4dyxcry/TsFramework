//!*******************************************************
//! TsWindowsUtility.h
//!
//! WindowsAPI Utility
//!
//! © 2016 Yuki Tsuneyama

#pragma once

#include <Windows.h>
#include "../TSUT/TsUT.h"

//! メインウィンドウを生成する
//  hIns        アプリケーションを起動した際のインスタンスデータ
//  cmd         アプリケーションを起動した際のコマンドラインデータ
//　windowSz    アプリケーションウィンドウのサイズ(指定しない場合はデフォルト)
TsBool TsWINCreateMainWindow( HINSTANCE hIns , TsInt cmd ,TsInt2 windowSz = TsInt2() );

//! MainWindowHandleの取得
HWND   TsWINGetMainHWND();

//! MainWindowのSizeを取得
TsInt2 TsWINGetWindowSize();

//! MainWindowのアスペクトを取得
TsF32  TsWINGetAspect();

//! ウィンドウの名前を設定(このメソッドはCreateする前にCallする必要があります。)
void   TsWINSetWindowTitle( TsString name );

//! ウィンドウプロシージャを設定(このメソッドはCreateする前にCallする必要があります。)
void   TsWINSetProc( WNDPROC proc);
 
