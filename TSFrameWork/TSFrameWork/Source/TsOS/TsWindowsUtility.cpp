#include "TsWindowsUtility.h"

// ! Prototype 
LRESULT CALLBACK TSDefaultWindowProc( HWND hWnd  , UINT message ,WPARAM wParam ,LPARAM lParam );

// ! Grobal Space
namespace 
{
    static	HWND	g_MainWindowHandle = nullptr;
    static  TsBool	g_isInitialize;

    static	const TsInt2	g_defaultWindowSize( 1980 , 1024 );
    static  WNDPROC g_windowProc = TSDefaultWindowProc;
    static  TsUint  g_windowStyle = CS_HREDRAW | CS_VREDRAW;

    static TsString g_windowName ="TsFramework";
}

//==============================================
//  Create Main Window
//==============================================
TsBool TsWINCreateMainWindow( HINSTANCE hIns , TsInt cmd , TsInt2 sz /* = 0,0 */ )
{
    WNDCLASSEX wc;

    //ウィンドウの初期設定
    wc.cbSize = sizeof( WNDCLASSEX );                   //ウィンドウのサイズを設定（メモリ）
    wc.style = g_windowStyle;                           //ウィンドウのスタイルを設定
    wc.hInstance = hIns;                                //アプリケーションのインスタンスを登録
    wc.lpszClassName = "TsFrameworkMainWindow";         //名前を設定
    wc.lpfnWndProc = g_windowProc;                      //プロシージャの登録
    wc.hIcon = LoadIcon( NULL , IDI_APPLICATION );      //アイコンを設定
    wc.hIconSm = LoadIcon( NULL , IDI_APPLICATION );
    wc.hCursor = LoadCursor( NULL , IDC_ARROW );        //カーソルを設定
    wc.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );  //BGカラーの設定
    wc.lpszMenuName = NULL;                             //メニューの名前を設定
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;

    if( !RegisterClassEx( &wc ) )
        return TS_FALSE;

    if( sz.x <= 0 || sz.y <= 0 )
        sz = g_defaultWindowSize;

    //ウィンドウを作成
    g_MainWindowHandle = CreateWindow(
        wc.lpszClassName ,		
        g_windowName.c_str() ,
        WS_OVERLAPPEDWINDOW ,	
        CW_USEDEFAULT ,
        CW_USEDEFAULT ,
        sz.x ,
        sz.y ,
        NULL ,
        NULL ,
        hIns ,
        NULL );

    if( g_MainWindowHandle == nullptr )
        return TS_FALSE;

    ShowWindow( g_MainWindowHandle , cmd );
    UpdateWindow( g_MainWindowHandle );

    return TS_TRUE;
}

//==============================================
//  Get MainWindow Handle
//==============================================
HWND   TsWINGetMainHWND()
{
    return g_MainWindowHandle;
}

//==============================================
//  Get MainWindow Size
//==============================================
TsInt2 TsWINGetWindowSize()
{
    RECT rect;
    GetWindowRect( g_MainWindowHandle , &rect );
    return TsInt2( rect.right - rect.left , 
                   rect.bottom - rect.top );
}

//! set window name
void TsWINSetWindowTitle( TsString name )
{
    g_windowName = name;
}

//! set window proc
void TsWINSetProc( WNDPROC proc )
{
    if( !g_isInitialize )
        g_windowProc = proc;
}

//Default windowproc
LRESULT CALLBACK TSDefaultWindowProc( HWND hWnd ,
                                      UINT message , 
                                      WPARAM wParam , 
                                      LPARAM lParam )
{
    switch( message )
    {
        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;
        default:
            return DefWindowProc( hWnd , message , wParam , lParam );
    }
    return 0;
}