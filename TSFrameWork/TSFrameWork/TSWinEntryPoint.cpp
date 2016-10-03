//=======================================================
//
//! 開発テスト用のエントリポイント
//
//　© Yuki Tsuneyama
//
//=======================================================
#include "TsAfx.h"
int APIENTRY WinMain( HINSTANCE hInstance , HINSTANCE 	hPrevInstance , LPSTR lpszArgs , TsInt nWinMode )
{
	TSUT::TsLoggerInit();

	TsApplicationBase api;
	api.Initialize(hInstance, nWinMode);
	TsDevice* pDev = api.GetDevice();

	TsRenderSystem rs;
	rs.LoadRenderSystemFromXML( pDev , "RenderSystem.ts_rs" );
	// ! Screen Space Test

	//load mesh
	TsDrawQueue queue;

	TsFbxContext fbx;


	TsMeshFactory factory;
	factory.LoadFromFile(pDev, "test.mqo");

	for( int i = 0; i < factory.GetPrimtiveNum(); ++i )
	{
		queue.Add(factory.CreateGeometryObject(i, pDev));
	}
	rs.SetDrawQue( &queue );

	TsCamera* pCamera = pDev->GetDC()->GetMainCamera();

	pCamera->CreateCBuffer(pDev);

	MSG msg;
	while( true )
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {

			//render 
			pDev->GetDC()->Clear();
			pCamera->UpdateForCBuffer(pDev);
			pDev->GetDC()->SetCBuffer(pCamera->GetCBuffer());

			rs.CallRender(pDev->GetDC());

			pDev->Flip();
		}
	};

	return 0;
}