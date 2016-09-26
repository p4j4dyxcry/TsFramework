//////////////////////////////////////////////////////////
//														//
// 	lesson01.cpp										//
// 		DirectX11グラフィックス初期化					//
//														//
//////////////////////////////////////////////////////////
#include <windows.h>

#include <stdio.h>
#include <process.h>	

#include "CDirectXGraphics.h" 

#include "Common.h"

// マクロの定義
#define NAME	"win32A"
#define TITLE	"SP42 lesson01 DirectX11 初期化 "

#define		SCREEN_X		((int)(1600.f * 0.75f))
#define		SCREEN_Y		((int)(900.f  * 0.75f))
#define		FULLSCREEN		0				// フルスクリーンフラグ

// リンクライブラリの設定
#pragma comment(lib, "dxgi.lib")			// リンク対象ライブラリにdxgi.libを追加
#pragma comment(lib, "d3d11.lib")			// リンク対象ライブラリにd3d11.libを追加
#pragma comment(lib, "winmm.lib")			// リンク対象ライブラリにwinmm.libを追加

// プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);			// ウインドウメッセージ関数
unsigned int   WINAPI GameMain(void  *);					// ゲームメイン関数関数
bool	GameInit(HINSTANCE ,HWND ,int ,int,bool );				// ゲームの初期処理
void	GameExit();											// ゲームの終了処理
void	CALLBACK TimerProc(UINT ,UINT,DWORD,DWORD,DWORD);	// タイマ処理

// グローバル変数
CDirectXGraphics	g_pD3D11;			// ＤＩＲＥＣＴＸ　ＧＲＡＰＨＩＣＳ　オブジェクト		

// ゲームメインスレッド関連
HANDLE			g_hEventHandle;		// イベントハンドル
int				g_timerid=0;		// タイマＩＤ
bool			g_EndFlag=false;	// 終了フラグ
HANDLE			g_hThread;			// スレッドのハンドル値
unsigned int	g_dwThreadId;		// スレッドＩＤ

#include "SampleMesh.h"

//////////////////////////
// ＷＩＮＭＡＩＮ関数	//
//////////////////////////
int APIENTRY WinMain(HINSTANCE 	hInstance, 		// アプリケーションインスタンス値
					 HINSTANCE 	hPrevInstance,	// 意味なし
					 LPSTR 		lpszArgs, 		// 起動時の引数文字列
					 int 		nWinMode)		// ウインドウ表示モード
{
	HWND			hwnd;						// ウインドウハンドル
	MSG				msg;						// メッセージ構造体
	WNDCLASSEX		wcex;						// ウインドウクラス構造体

	int				width;						// ウインドウの幅
	int				height;						// ウインドウの高さ
	CoInitializeEx( nullptr , COINIT_MULTITHREADED );

	// ウインドウクラス情報のセット
	wcex.hInstance		= hInstance;			// インスタンス値のセット
	wcex.lpszClassName	= NAME;					// クラス名
	wcex.lpfnWndProc	= (WNDPROC)WndProc;		// ウインドウメッセージ関数
	wcex.style			= 0;					// ウインドウスタイル
	wcex.cbSize 		= sizeof(WNDCLASSEX);	// 構造体のサイズ
	wcex.hIcon			= LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);	// ラージアイコン
	wcex.hIconSm		= LoadIcon((HINSTANCE)NULL, IDI_WINLOGO);		// スモールアイコン
	wcex.hCursor		= LoadCursor((HINSTANCE)NULL, IDC_ARROW);		// カーソルスタイル
	wcex.lpszMenuName	= 0; 					// メニューなし
	wcex.cbClsExtra		= 0;					// エキストラなし
	wcex.cbWndExtra		= 0;					
	wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);		// 背景色白

	if (!RegisterClassEx(&wcex)) return FALSE;	// ウインドウクラスの登録

	timeBeginPeriod(1);			// タイマの分解能力を１ｍｓにする
	g_timerid = timeSetEvent(16,1,TimerProc,1,TIME_PERIODIC);

	// ウインドウサイズを計算
	if(FULLSCREEN){
		width = SCREEN_X;
		height = SCREEN_Y;
	}
	else{
		width = SCREEN_X+GetSystemMetrics(SM_CXDLGFRAME)*2;
		height = SCREEN_Y+GetSystemMetrics(SM_CYDLGFRAME)*2+GetSystemMetrics(SM_CYCAPTION);						// ウインドウサイズ
	}

	hwnd = CreateWindow(
				NAME, 							// ウインドウクラスの名前
				TITLE, 							// タイトル
				WS_CAPTION | WS_SYSMENU,		// ウインドウスタイル
				100,100,						// X座標,Y座標
				width,height,					// ウインドウサイズ
				HWND_DESKTOP, 					// 親ウインドウなし
				(HMENU)NULL, 					// メニューなし
				hInstance, 						// インスタンスハンドル
				(LPVOID)NULL);					// 追加引数なし

	if (!hwnd) return FALSE;


	// ゲームの初期処理
	if (!GameInit(hInstance, hwnd, width, height,FULLSCREEN)){
		GameExit();
		MessageBox(hwnd, "ERROR!", "GameInit Error", MB_OK);
		return false;
	}

	// ウインドウを表示する
	ShowWindow(hwnd, nWinMode);
	UpdateWindow(hwnd);

	// メッセージ･ループ
	while(1){
		// メッセージを取得
		if( !GetMessage( &msg, NULL, 0, 0 ) ) 
		{ 
			break; 
		} 
		else{ 
			TranslateMessage(&msg); 		// キーボード使用を可能にする
			DispatchMessage(&msg); 			// コントロールをＷＩＮＤＯＷＳに戻す
		} 
	}

	g_EndFlag=true;									// 終了フラグをオンにする

	if(g_timerid) timeKillEvent(g_timerid);			// マルチメディアタイマの終了
	timeEndPeriod(1);								// タイマの分解能力もとに戻す

	// ゲームの終了処理
	GameExit();

	return (int)msg.wParam;
}
//////////////////////////
// ゲームメイン関数		//
//////////////////////////
unsigned int	WINAPI GameMain(void* p1)
{
	SampleLoad();
	while(1){
		// イベントフラグがセットされるのを待つ　タイムアウト１０００ｍｓ
		DWORD sts=WaitForSingleObject(g_hEventHandle,1000);
		if(sts==WAIT_FAILED){
			break;
		}else if(sts == WAIT_TIMEOUT){
			if(g_EndFlag){
				break;
			}
			continue;
		}

		float ClearColor[4] = { 0, 0, 1.0f, 1.0f }; //red,green,blue,alpha

		// ターゲットバッファクリア
	    g_pD3D11.GetImmediateContext()->ClearRenderTargetView( g_pD3D11.GetRenderTargetView(), ClearColor );

		SampleDraw();

		// SWAP
		g_pD3D11.GetSwapChain()->Present( 0, 0 );

	}
	return 1;
}

////////////////////////////////////////////////////////////////
// メッセージ処理関数（ウインドウ関数）                       //
//															  //
//	この関数は、ＯＳから呼び出されてメッセージキューから      //
//	メッセージの引渡しを受ける								  //
//															  //
////////////////////////////////////////////////////////////////
LRESULT WINAPI WndProc(HWND hwnd, 				// ウインドウハンドル値
						UINT message,			// メッセージ識別子
						WPARAM wParam,			// 付帯情報１
						LPARAM lParam)			// 付帯情報２
{
	switch (message){
		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

//////////////////////////
// ゲームの初期化処理	//
//////////////////////////
bool GameInit(HINSTANCE hinst,HWND hwnd,int width,int height,bool fullscreen){

	bool		sts;		

	// ＤｉｒｅｃｔＸ　Ｇｒａｐｈｉｃｓオブジェクト初期化
	sts = g_pD3D11.Init(hwnd,FULLSCREEN,width,height,fullscreen);
	if(!sts){		
		MessageBox(hwnd,"ERROR!!","DirectX 初期化エラー",MB_OK);
		return false;
	}

	// イベントハンドル生成
	g_hEventHandle=CreateEvent(NULL,false,false,NULL);
	if(g_hEventHandle==NULL){
		MessageBox(hwnd,"ERROR!!","CreateEvent エラー",MB_OK);
		return false;
	}

	// スレッド生成(ゲームメイン)
	g_hThread = (HANDLE)_beginthreadex(NULL,0,GameMain,0,0,&g_dwThreadId);
	if(g_hThread==0){
		MessageBox(hwnd,"ERROR!!","beginthreadex エラー",MB_OK);
		return false;
	}

	return	true;
}

//////////////////////////
// ゲームの終了処理		//
//////////////////////////
void GameExit(){

	SampleExit();
	// ゲームメインスレッドの終了を待つ
	WaitForSingleObject(g_hThread,INFINITE);

	// スレッドハンドルをクローズ
	CloseHandle(g_hThread);

	// イベントハンドルクローズ
	CloseHandle(g_hEventHandle);

	g_pD3D11.Exit();

}

//////////////////////////
//	タイマーＰＲＯＣ	//
//////////////////////////
void CALLBACK TimerProc(UINT ,UINT,DWORD,DWORD,DWORD){
	// 終了フラグがセットされていなければイベントをセットする
	if(!g_EndFlag){
		SetEvent(g_hEventHandle);
	}
}
