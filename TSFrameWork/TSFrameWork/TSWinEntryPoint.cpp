//=======================================================
//
//! 開発テスト用のエントリポイント
//
//　© Yuki Tsuneyama
//
//=======================================================
#include "TsAfx.h"
#include "Source\TsOS\TsWindowsUtility.h"
void TestUpdateCamera(TsCamera* pCamera)
{
    static TsFloat2 old;
    TsFloat2 pos = TsWINGetMousePos();

    TsInt wheel = TsWINGetMouseWheel();
    TsVector3 diff;
    diff.y = pos.x - old.x;
    diff.x = pos.y - old.y;


    if (TsWINIsLeftClick())
    {
        TsVector3 euler = pCamera->GetLocalRotate().ToEuler();
        euler += diff;
        pCamera->SetLocalRotate( TsQuaternion::CreateByEuler(euler)) ;
    }


    TsF32 fov = pCamera->GetFov();
    fov += wheel * 0.01f;
    fov = TsClamp(fov, 5.0f, 80.0f);
    pCamera->SetFov(fov);

    old = pos;
}

int APIENTRY WinMain( HINSTANCE hInstance , HINSTANCE 	hPrevInstance , LPSTR lpszArgs , TsInt nWinMode )
{
    TSUT::TsLoggerInit();

    TsDirectioalLight dir;
    dir.LookAt(TsVector3::front * 100, TsVector3::back, TsVector3::up);
    auto mat = dir.GetWorldMatrix();
    TsApplicationBase api;
    api.Initialize(hInstance, nWinMode);
    TsDevice* pDev = api.GetDevice();

    auto p = TsDirectXTex::LoadFromFile(pDev->GetDevD3D(), "cubemap.dds");
    ID3D11Resource* ptr;
    p.pSrv->GetResource( &ptr );
    D3D11_SHADER_RESOURCE_VIEW_DESC desc;
    p.pSrv->GetDesc( &desc );

    TsCubeMap* pTex = TsNew( TsCubeMap) ;
    pTex->SetSRV(p.pSrv);

    //TsSkyBox * pSkyBox = new TsSkyBox;
    //pSkyBox->Create(pDev);
    //pSkyBox->SetCubeMap( pTex );

    TsResourceManager::Initialize( pDev );
    TsRenderSystem rs;
    rs.LoadRenderSystemFromXML( pDev , "RenderSystem.ts_rs" );
    // ! Screen Space Test

    //load mesh
    TsDrawQueue queue;
    //queue.Add( pSkyBox );

    TsMeshFactory factory;
//     factory.LoadModelFromFile(pDev, "Resource/fbx/Unity-Chan/unitychan.fbx","Test");
//     factory.LoadModelFromFile( pDev , "Resource/fbx/miku/miku.fbx" );
//     factory.LoadModelFromFile( pDev , "Idol.fbx","Test" );
//     factory.LoadModelFromFile(pDev, "SD_unitychan_generic.fbx","Test");
       factory.LoadModelFromFile(pDev, "Face.fbx","Test");
//     auto pAnim = factory.CreateBakeAnimation( "move.fbx");
//     auto pAnim = factory.CreateBakeAnimation( "Resource/fbx/Unity-Chan/move_unity.fbx" );
//     auto pAnim = factory.CreateBakeAnimation( "sd_anim.fbx" );
     TsMeshObject * pMesh = TsResourceManager::Find<TsMeshObject>("Test");
//     pAnim->BindTransform( pMesh->GetGeometry( 0 )->GetTransform()->GetRootTransform() );

     TsSkeleton* pSkeleton = pMesh->GetSkeleton();

//     pAnim->SetTargetSkeleton( pSkeleton );
    for (TsInt i = 0; i < pMesh->GetGeometryCount(); ++ i)   
         queue.Add(pMesh->GetGeometry(i));

    rs.SetDrawQue( &queue );

    TsCamera* pCamera = pDev->GetDC()->GetMainCamera();

//    pCamera->SetLocalRotate( TsQuaternion::AngleAxis( TsVector3::up , TsRadian( 180.0f ) ) );
    pCamera->SetLocalPosition(TsVector3(0,0,-100));
    pCamera->SetLockAt( TsVector3( TsVector3( 0 , 0 , 0 ) ) );
    pCamera->SetNearAndFar(30, 700);

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
            
            auto pBlendState = TsResourceManager::Find<TsBlendState>( "ALPHA_BLEND" );
            pDev->GetDC()->SetBlendState( pBlendState );
            pDev->GetDC()->ApplyBlendState();

            TestUpdateCamera(pCamera);
            pCamera->UpdateForCBuffer(pDev);
            pDev->GetDC()->SetCBuffer(pCamera->GetCBuffer());

            rs.CallRender(pDev->GetDC());

            pDev->Flip();
        }
    };

    return 0;
}