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
     factory.LoadModelFromFile(pDev, "Resource/fbx/Unity-Chan/unitychan.fbx","Test");
     //factory.LoadModelFromFile( pDev , "Resource/fbx/miku/miku.fbx" );
     //factory.LoadModelFromFile( pDev , "Idol.fbx","Test" );
     //factory.LoadModelFromFile(pDev, "SD_unitychan_generic.fbx","Test");
     auto pAnim = factory.CreateBakeAnimation( "move_unity.fbx");
     TsMeshObject * pMesh = TsResourceManager::Find<TsMeshObject>("Test");
     pAnim->BindTransform( pMesh->GetGeometry( 0 )->GetTransform()->GetRootTransform() );
     for (TsInt i = 0; i < pMesh->GetGeometryCount(); ++ i)   
         queue.Add(pMesh->GetGeometry(i));

    rs.SetDrawQue( &queue );

    TsCamera* pCamera = pDev->GetDC()->GetMainCamera();

    pCamera->SetEyePosition(TsVector3(0,70,200));
    pCamera->SetLookAtVector( TsVector3(0,70,0));
    pCamera->CreateCBuffer(pDev);
    pCamera->SetNearAndFar(1, 2000);

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
            pAnim->Update();
            pDev->GetDC()->Clear();
            pCamera->UpdateForCBuffer(pDev);
            pDev->GetDC()->SetCBuffer(pCamera->GetCBuffer());

            rs.CallRender(pDev->GetDC());

            pDev->Flip();
        }
    };

    return 0;
}