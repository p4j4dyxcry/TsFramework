#include "TsCore.h"
#include "../TsOS/TsWindowsUtility.h"
#include <thread>

TsApplicationBase::TsApplicationBase() :
m_pGfxDevice( nullptr ) 
{

}

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