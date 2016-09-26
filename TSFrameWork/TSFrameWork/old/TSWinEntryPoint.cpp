#include "Source\TsOS\TsWindowsUtility.h"
#include "Source\TsGfx\TsGfx.h"
int APIENTRY WinMain( HINSTANCE hInstance , HINSTANCE 	hPrevInstance , LPSTR lpszArgs , int nWinMode )
{
	TSUT::TsLoggerInit();
	TsWINCreateMainWindow( hInstance , nWinMode );
	TsDevice device;

	TsInt2 windowSz = TsWINGetWindowSize();
	device.CreateDevice( TsWINGetMainHWND() , windowSz.x , windowSz.y );

	TsPixelShader d;
	d.LoadFromCSO( device.GetDevD3D() , ( TSUT::GetCSODirectory() + "TsSSAddPS.cso" ).c_str() );
	TsVertexDefault vertex[3];

	vertex[0].pos = TsVector3( 0 , 0.5f , 0 );
	vertex[1].pos = TsVector3(-0.5f , 0 , 0 );
	vertex[2].pos = TsVector3( 0.5f , 0 , 0 );

	MSG msg;
	while( true )
	{
		if( PeekMessage( &msg , NULL , 0 , 0 , PM_REMOVE ) )
		{
			if( msg.message == WM_QUIT )
				break;
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			device.GetDC()->Clear();
		}
	};
	return 0;
}