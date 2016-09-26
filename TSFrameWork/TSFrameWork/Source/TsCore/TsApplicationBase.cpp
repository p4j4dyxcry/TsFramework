#include "TsCore.h"
#include "../TsOS/TsWindowsUtility.h"
#include <thread>

TsApplicationBase::TsApplicationBase() :
m_pGfxDevice( nullptr ) 
{

}
/*

std::thread t1(process); // 別スレッドでprocessの処理を行う
std::thread t2([&i](int x) {
int counter = 0;
while(counter++ < x) {
i += counter;
}
}, 10); // 関数オブジェクトの引数に10を渡す
t1.detach(); //スレッドt1の管理を放棄
t2.join(); //スレッドt2の処理が終了するまで待機
std::cout << i << std::endl;

*/
TsBool TsApplicationBase::Initialize( HINSTANCE hInstance , TsInt nWinMode )
{
	TSUT::TsLoggerInit();
	TsBool bResult = TsWINCreateMainWindow( hInstance , nWinMode );

	if( bResult == TS_FALSE )return TS_FALSE;

	TsInt2 windowSz = TsWINGetWindowSize();
	m_pGfxDevice = TsNew( TsDevice );
	bResult = m_pGfxDevice->CreateDevice( TsWINGetMainHWND() , windowSz.x , windowSz.y );

	if (bResult == TS_FALSE)return TS_FALSE;
	/*
	auto MessageLoop = []()
	{
		MSG msg;

		while (true)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	};
	static std::thread thread(MessageLoop);
	*/
	//thread.join();

	return TS_TRUE;
}

TsDevice* TsApplicationBase::GetDevice()
{
	return m_pGfxDevice;
}