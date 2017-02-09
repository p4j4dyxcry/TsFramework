#include "TsUT.h"
#include "../TsOS/TsWindowsUtility.h"
#include	<fstream>
#pragma warning(disable: 4996)

//======================================
//
// ! Grobal Space
//
//======================================
namespace
{
    HANDLE      g_hTsLoggerInput;
    HANDLE      g_hTsLoggerOutput;
    HWND        g_hLogger;

    TsString    g_OutputFileName = "";

    TsBool      g_bShowConsole;
    TsBool      g_bWriteLog;

    TsVector<void*> g_allocList;
}
//======================================
// ! FindConsoleHandle()
//======================================
static HWND FindConsoleHandle()
{
    const int titleBuffer = 256;
    HWND hwndFound;

    TCHAR pszNewWindowTitle[titleBuffer];
    TCHAR pszOldWindowTitle[titleBuffer];

    // 現在のウィンドウタイトルを取得します
    GetConsoleTitle( pszOldWindowTitle , titleBuffer );

    // 独自に、ウィンドウの新規タイトルをフォーマットします
    wsprintf( pszNewWindowTitle , "%d/%d" ,
              GetTickCount() ,
              GetCurrentProcessId() );
    SetConsoleTitle( pszNewWindowTitle );
    Sleep( 100 );
    hwndFound = FindWindow( "ConsoleWindowClass" , pszNewWindowTitle );
    SetConsoleTitle( pszOldWindowTitle );
    return( hwndFound );
}

void SetConsoleFontSize(TsInt sz)
{
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;                   // Width of each character in the font
    cfi.dwFontSize.Y = sz;                  // Height
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    std::wcscpy(cfi.FaceName, L"メイリオ"); // Choose your font
    SetCurrentConsoleFontEx(g_hTsLoggerOutput, FALSE, &cfi);
}

//======================================
// ! CreateConsole()
//======================================
static void CreateConsole()
{
    //コンソール作成
    AllocConsole();

    freopen( "CON" , "r" , stdin );     // 標準入力の割り当て
    freopen( "CON" , "w" , stdout );    // 標準出力の割り当て

    // Get Handle
    g_hTsLoggerOutput = GetStdHandle( STD_OUTPUT_HANDLE );
    g_hTsLoggerInput = GetStdHandle( STD_INPUT_HANDLE );

    //バッファサイズと描画領域設定
    COORD crd = { 128 , 9999 };
    SetConsoleScreenBufferSize( g_hTsLoggerOutput , crd );

    g_hLogger = FindConsoleHandle();
    RECT rect;

    TsInt dispx = GetSystemMetrics(SM_CXSCREEN);
    TsInt dispy = GetSystemMetrics(SM_CYSCREEN);

    // - 16 ~ 16
    TsInt fontSize = 10;
    GetWindowRect(g_hLogger, &rect); //stores the console's current dimensions
    MoveWindow(g_hLogger, dispx - 768, 0, 1024, dispy * 1.6 , TRUE);
    SetConsoleFontSize(fontSize);
    SetConsoleTitle( "Ts Debug Logger" );
}


//======================================
// ! TSUT::TsLoggerInit()
//======================================
TsBool TSUT::TsLoggerInit( TsBool showConsole		/*= TS_TRUE*/  ,
                           TsBool writeLog			/*= TS_TRUE*/  ,
                           TsString outputFileName  /*= "Debug"*/  ,
                           TsString outputDir		/*= ""	*/	 )
{
    if( g_bShowConsole )		
        return TS_FALSE;

    if( (showConsole & writeLog) == TS_FALSE )
        return TS_FALSE;

    g_bShowConsole = showConsole;
    g_bWriteLog = writeLog;

    if( g_bShowConsole )
        CreateConsole();

    g_OutputFileName = outputDir + outputFileName + ".log";

    std::ofstream ofs( g_OutputFileName , std::ios::out | std::ios::trunc );

    return TS_TRUE;
}

void TSUT::TsConsoleColor(TS_CONSOLE_COLOR color)
{
    SetConsoleTextAttribute(g_hTsLoggerOutput, (WORD)color);
}

//======================================
// ! TSUT::TsLog()
//======================================
void TSUT::TsLog( const char * fmt , ... )
{
    char buffer[1024] = "";
    va_list arg;

    va_start( arg , fmt );
    if( g_bShowConsole )
        ::vprintf( fmt , arg );
    if( g_bWriteLog )
        ::vsnprintf( buffer , 1024 , fmt , arg );
    va_end( arg );

    if( buffer[0] != '\0' )
    {
        std::ofstream ofs( g_OutputFileName , std::ios::out | std::ios::app );
        ofs << buffer;
        OutputDebugString( buffer );
    }
}

//======================================
//! 暗黙の終了処理
//======================================
struct TsFreeConsole
{
private:
    TsFreeConsole(){};
    TsFreeConsole( const TsFreeConsole& ){};
    TsFreeConsole operator = ( const TsFreeConsole& ){};
    static TsFreeConsole m_close;

    ~TsFreeConsole()
    {
        if( g_bShowConsole )
            FreeConsole();
    }
};
TsFreeConsole TsFreeConsole::m_close;