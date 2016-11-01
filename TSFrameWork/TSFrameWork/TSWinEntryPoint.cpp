﻿//=======================================================
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

    auto p = TsDirectXTex::LoadFromFile(pDev->GetDevD3D(), "cubemap.dds");
    ID3D11Resource* ptr;
    p->GetResource(&ptr);
    D3D11_SHADER_RESOURCE_VIEW_DESC desc;
    p->GetDesc(&desc);

    TsCubeMap* pTex = TsNew( TsCubeMap) ;
    pTex->SetSRV(p);

    TsSkyBox * pSkyBox = new TsSkyBox;
    pSkyBox->Create(pDev);
    pSkyBox->SetCubeMap( pTex );

    TsResourceManager::Initialize( pDev );
    TsRenderSystem rs;
    rs.LoadRenderSystemFromXML( pDev , "RenderSystem.ts_rs" );
    // ! Screen Space Test

    //load mesh
    TsDrawQueue queue;
    queue.Add( pSkyBox );

    TsMeshFactory factory;
//     factory.LoadModelFromFile(pDev, "Resource/fbx/Unity-Chan/unitychan.fbx","Test");
//     factory.LoadModelFromFile( pDev , "Resource/fbx/miku/miku.fbx" );
//     factory.LoadModelFromFile( pDev , "Idol.fbx","Test" );
//     factory.LoadModelFromFile(pDev, "SD_unitychan_generic.fbx","Test");
//     auto pAnim = factory.CreateBakeAnimation( "move.fbx");
//     auto pAnim = factory.CreateBakeAnimation( "move_unity.fbx" );
//     auto pAnim = factory.CreateBakeAnimation( "sd_anim.fbx" );
//     TsMeshObject * pMesh = TsResourceManager::Find<TsMeshObject>("Test");
//     pAnim->BindTransform( pMesh->GetGeometry( 0 )->GetTransform()->GetRootTransform() );

//     TsSkeleton* pSkeleton = pMesh->GetSkeleton();

//     pAnim->SetTargetSkeleton( pSkeleton );
//     for (TsInt i = 0; i < pMesh->GetGeometryCount(); ++ i)   
//         queue.Add(pMesh->GetGeometry(i));

    rs.SetDrawQue( &queue );

    TsCamera* pCamera = pDev->GetDC()->GetMainCamera();

    pCamera->SetEyePosition(TsVector3(0,0,-1));
    pCamera->SetLookAtVector( TsVector3(0,0,1));
    pCamera->SetNearAndFar(0.001f, 500);

    pCamera->CreateCBuffer(pDev);

    TsDrawQueue postQue;
    TsScreenObject * pScreen = TsNew( TsScreenObject );
    pScreen->Create(pDev);
    postQue.Add( pScreen );

    rs.SetDrawQue( &postQue , TsRenderSystem::TARGET_FLOW::POST_RENDERER );
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
//            pAnim->Update();

            pCamera->UpdateForCBuffer(pDev);
            pDev->GetDC()->SetCBuffer(pCamera->GetCBuffer());

            rs.CallRender(pDev->GetDC());

            pDev->Flip();
        }
    };

    return 0;
}