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
    TsResourceManager::Initialize( pDev );
    TsRenderSystem rs;
    rs.LoadRenderSystemFromXML( pDev , "RenderSystem.ts_rs" );
    // ! Screen Space Test

    //load mesh
    TsDrawQueue queue;

    TsMeshFactory factory;
     //factory.LoadFromFile(pDev, "Resource/fbx/Unity-Chan/unitychan.fbx");
     factory.LoadFromFile( pDev , "Resource/fbx/miku/miku.fbx" );
     //factory.LoadFromFile( pDev , "Idol.fbx" );


    for( int i = 0; i < factory.GetPrimtiveNum(); ++i )
    {
        queue.Add(factory.CreateGeometryObject(i, pDev));
    }
    rs.SetDrawQue( &queue );

    TsCamera* pCamera = pDev->GetDC()->GetMainCamera();

    pCamera->SetEyePosition(TsVector3(0,70,200));
    pCamera->SetLookAtVector( TsVector3(0,70,0));
    pCamera->CreateCBuffer(pDev);
    pCamera->SetNearAndFar(1, 2000);

    // test of animation
    TsBoneCBuffer boneCBuffer;
    boneCBuffer.CreateBoneCBuffer( pDev );
    TsGeometryObject* pGeo = dynamic_cast< TsGeometryObject*>(queue.FindGeometoryByIndex( 0 ));
    TsBoneTransForm* pBoneRoot = 
        (TsBoneTransForm*) pGeo->GetTransform()->FindChildByClassName( "TsBoneTransForm" );
    boneCBuffer.SetRootBoneTransform( pBoneRoot );


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
            boneCBuffer.UpdateCBuffer( pDev->GetDC() );
            boneCBuffer.ApplyCBuffer( pDev->GetDC() );
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