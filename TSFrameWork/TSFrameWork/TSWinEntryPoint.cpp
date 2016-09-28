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
	TsHDRTextureAnalizer hdr;
	hdr.LoadHDR( "galileo_probe.hdr" );

	TsApplicationBase api;
	api.Initialize(hInstance, nWinMode);
	TsDevice* pDev = api.GetDevice();
	TsResourceManager mgr;
	//mgr.LoadShaders( pDev );

	//shader to rendering pass
	TsRenderPass pass;

	pass.LoadShaderFromXML(pDev,"LightPrePass.ts_pass");

	pass.SetInputSlot(0, 0);
	//setrender target
	pass.SetOutputSlot(0, pDev->GetDC()->GetScreenRTV());

	TsDrawQueue queue;
	TsRenderFlow flow;
	flow.SetRenderPass(&pass);

	TsRenderSystem rs;
	rs.LoadShaderResourceFromXML(pDev, "shaderResouce.ts_shaderReource");
	rs.SetDrawQue(&queue);
	rs.SetShaderFlow(&flow);
	// ! Screen Space Test

	//load mesh
	TsMeshFactory factory;
	factory.LoadFromFile(pDev, "test.mqo");

	for( int i = 0; i < factory.GetPrimtiveNum(); ++i )
	{
		queue.Add(factory.CreateGeometryObject(i, pDev));
	}

	pass.SetDrawQueue( &queue );
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