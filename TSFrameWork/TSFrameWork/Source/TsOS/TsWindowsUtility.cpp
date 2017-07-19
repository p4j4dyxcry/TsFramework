#include "TsWindowsUtility.h"

// ! Prototype 
LRESULT CALLBACK TSDefaultWindowProc( HWND hWnd  , UINT message ,WPARAM wParam ,LPARAM lParam );

// ! Grobal Space
namespace 
{
    HWND	g_MainWindowHandle = nullptr;
    TsBool	g_isInitialize;

    const TsInt2	g_defaultWindowSize( 1980 , 1024 );
    WNDPROC g_windowProc = TSDefaultWindowProc;
    TsUint  g_windowStyle = CS_HREDRAW | CS_VREDRAW;

    TsString g_windowName ="TsFramework";

    TsBool   g_IsLClick;
    TsBool   g_IsRClick;
    TsBool   g_IsCClick;
    TsBool   g_keyDown[128];

    void DefaultTsWinWheelFunc(TsF32 delta){(void)delta;}

    void(*g_DefaultTsWinWheelFunc)(TsF32 delta) = DefaultTsWinWheelFunc;
}

//==============================================
//  Create Main Window
//==============================================
TsBool TsWINCreateMainWindow( HINSTANCE hIns , TsInt cmd , TsInt2 sz /* = 0,0 */ )
{
    WNDCLASSEX wc;

    //�E�B���h�E�̏����ݒ�
    wc.cbSize = sizeof( WNDCLASSEX );                   //�E�B���h�E�̃T�C�Y��ݒ�i�������j
    wc.style = g_windowStyle;                           //�E�B���h�E�̃X�^�C����ݒ�
    wc.hInstance = hIns;                                //�A�v���P�[�V�����̃C���X�^���X��o�^
    wc.lpszClassName = "TsFrameworkMainWindow";         //���O��ݒ�
    wc.lpfnWndProc = g_windowProc;                      //�v���V�[�W���̓o�^
    wc.hIcon = LoadIcon(nullptr , IDI_APPLICATION );      //�A�C�R����ݒ�
    wc.hIconSm = LoadIcon(nullptr , IDI_APPLICATION );
    wc.hCursor = LoadCursor(nullptr , IDC_ARROW );        //�J�[�\����ݒ�
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);  //BG�J���[�̐ݒ�
    wc.lpszMenuName = nullptr;                             //���j���[�̖��O��ݒ�
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;

    if( !RegisterClassEx( &wc ) )
        return TS_FALSE;

    if( sz.x <= 0 || sz.y <= 0 )
        sz = g_defaultWindowSize;

    //�E�B���h�E���쐬
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
void TsWINTsWINSetProc(WNDPROC proc)
{
    if( !g_isInitialize )
        g_windowProc = proc;
}

TsBool TsWINGetKey(TsU8 key)
{
    return g_keyDown[key];
}

TsFloat2 TsWINGetMousePos()
{
    POINT p;
    GetCursorPos(&p);
    return TsFloat2(p.x, p.y);
}

TsBool TsWINIsLeftClick()
{
    return g_IsLClick;
}

TsBool TsWINIsRightCkick()
{
    return g_IsRClick;
}

TsBool TsWINIsCenterCkick()
{
    return g_IsCClick;
}

void SetTsWIN_MouseWheelFunc(void(*func)(TsF32 delta))
{
    g_DefaultTsWinWheelFunc = func;
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
        case WM_LBUTTONDOWN:
            g_IsLClick = TS_TRUE;
            break;
        case WM_MOUSEWHEEL:
        {
            TsF32 delta = GET_WHEEL_DELTA_WPARAM(wParam);
            g_DefaultTsWinWheelFunc(delta);
        }
            break;
        case WM_LBUTTONUP:
            g_IsLClick = TS_FALSE;
            break;
        case WM_RBUTTONDOWN:
            g_IsRClick = TS_TRUE;
            break;
        case WM_RBUTTONUP:
            g_IsRClick = TS_FALSE;
            break;
        case WM_KEYDOWN:
            g_keyDown[wParam] = TS_TRUE;
            break;
        case WM_KEYUP:
            g_keyDown[wParam] = TS_FALSE;
            break;
        case WM_MBUTTONDOWN:
            g_IsCClick = TS_TRUE;
            break;
        case WM_MBUTTONUP:
            g_IsCClick = TS_FALSE;
            break;
        default:
            return DefWindowProc( hWnd , message , wParam , lParam );
    }
    return 0;
}