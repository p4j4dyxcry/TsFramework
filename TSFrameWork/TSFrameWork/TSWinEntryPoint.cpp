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

    TsTransForm root;
    TsTransForm child[3];
    child[0].m_localPosition.y = 2;
    child[0].m_localRotate = TsQuaternion::Euler( TsVector3( 0 , 0 , 90 ) );


    float r = TsRadian( 90 );
    r = TsDegree( r );

    child[1].m_localPosition.z = 2;
    child[1].m_localRotate = TsQuaternion::Euler( TsVector3( 0 , 90 , 0 ) );

    child[2].m_localPosition.x = 1;
    child[2].m_localRotate = TsQuaternion::Euler( TsVector3( 90 , 0 , 0 ) );

    child[2].SetParent( &child[1] );
    child[1].SetParent( &child[0] );
    child[0].SetParent( &root );
    TsTransForm ans1 =  child[0].ToWorldMatrix();
    TsTransForm ans2 =  child[1].ToWorldMatrix();
    TsTransForm ans3 =  child[2].ToWorldMatrix();
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
//    factory.LoadFromFile(pDev, "Resource/fbx/Unity-Chan/unitychan.fbx");
//    factory.LoadFromFile( pDev , "Resource/fbx/miku/miku.fbx" );
 factory.LoadFromFile( pDev , "Idol.fbx" );


    for( int i = 0; i < factory.GetPrimtiveNum(); ++i )
    {
        queue.Add(factory.CreateGeometryObject(i, pDev));
    }
    rs.SetDrawQue( &queue );

    TsCamera* pCamera = pDev->GetDC()->GetMainCamera();

    pCamera->SetEyePosition(TsVector3(0,50,500));
    pCamera->SetLookAtVector( TsVector3(0,50,0));
    pCamera->CreateCBuffer(pDev);
    pCamera->SetNearAndFar(1, 2000);

    TsBoneCBuffer boneCBuffer;
    boneCBuffer.CreateBoneCBuffer( pDev );
    TsGeometryObject* pGeo = dynamic_cast< TsGeometryObject*>(queue.FindGeometoryByIndex( 0 ));
    TsBoneTransForm* pBoneRoot = 
        (TsBoneTransForm*) pGeo->GetTransform()->FindChildByClassName( "TsBoneTransForm" );
    auto pTest = TsNew( TsTransForm );
    boneCBuffer.SetWorldTransform( pTest );
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