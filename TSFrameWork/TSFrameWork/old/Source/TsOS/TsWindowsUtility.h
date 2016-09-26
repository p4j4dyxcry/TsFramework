//!*******************************************************
//!	TsWindowsUtility.h
//!
//!	WindowsAPI Utility
//!
//!	© 2016 Yuki Tsuneyama

#pragma once

#include <Windows.h>
#include "../TSUT/TsUT.h"

TsBool TsWINCreateMainWindow( HINSTANCE hIns , TsInt cmd ,TsInt2 windowSz = TsInt2() );
HWND   TsWINGetMainHWND();
TsInt2 TsWINGetWindowSize();
TsF32  TsWINGetAspect();
void   TsWINSetWindowTitle( TsString name );
void   TsWINSetProc( WNDPROC proc);
 
