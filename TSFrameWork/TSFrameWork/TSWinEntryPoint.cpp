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

//    dir.LookAt(TsVector3::front * 100, TsVector3::back, TsVector3::up);
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

    TsLightSetCBuffer * pLightSetCB = TsNew(TsLightSetCBuffer);
    pLightSetCB->CreateLightSetCBuffer( pDev );
    TsDirectioalLight * pLight = TsNew(TsDirectioalLight);
    pLight->SetLocalRotate( TsQuaternion::CreateByEuler( 0 , -75 , -75 ) );
    pLightSetCB->AddLight( pLight );

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
     pMesh->GetGeometry( 0 )->GetTransform()->GetRootTransform()->m_localScale = TsVector3::one * 0.1f;
    for (TsInt i = 0; i < pMesh->GetGeometryCount(); ++ i)   
         queue.Add(pMesh->GetGeometry(i));

    TsPlaneObject plane;
    TsTransForm planeTransform;
    planeTransform.m_localScale = TsVector3( 500 , 1 , 500 );
    plane.Create( pDev );
    plane.SetTransform( &planeTransform );
    queue.Add(&plane);

    TsAABB3D aabb(TsVector3(-15, 0, -15), TsVector3(15, 30, 15));
    TsLine3D line(TsVector3(-15, -15, -15), TsVector3(15, 15, 15));
    TsOBB    obb;
    obb.SetScale(TsVector3(10, 5, 5));
    obb.SetRotate(TsQuaternion::CreateByEuler(45, 45, 0));
    obb.SetCenter(TsVector3(15, 2.5f, 0));
    TsColliderRenderObject obbMesh;
    obbMesh.CreateRenderObject(pDev, &obb);

    TsColliderRenderObject aabbMesh;
    aabbMesh.CreateRenderObject(pDev, &aabb);
    queue.Add(&aabbMesh);

    queue.Add(&obbMesh);

    TsSphere3D sphere;
    sphere.SetRadius(15);
    TsColliderRenderObject sphereMesh;
    sphereMesh.CreateRenderObject(pDev, &sphere);

    queue.Add(&sphereMesh);

    rs.SetDrawQue( &queue );

    TsCamera* pCamera = pDev->GetDC()->GetMainCamera();

//    pCamera->SetLocalRotate( TsQuaternion::AngleAxis( TsVector3::up , TsRadian( 180.0f ) ) );
    pCamera->SetLocalPosition(TsVector3(0,4,-50));
    pCamera->SetLockAt( TsVector3( TsVector3( 0 , 4 , 0 ) ) );

    pCamera->SetNearAndFar(4, 255);

    pCamera->CreateCBuffer(pDev);

    TsDrawQueue postQue;
    TsScreenObject * pScreen = TsNew( TsScreenObject );
    pScreen->Create(pDev);
    postQue.Add( pScreen );

    rs.SetDrawQue( &postQue , TsRenderSystem::TARGET_FLOW::POST_RENDERER );
    MSG msg;

    TsVector3 obbCenter = TsVector3(15, 2.5f, 0);
    TsVector3 obbEuler = TsVector3(45, 45, 0);


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
            
            if (TsWINGetKey(VK_LEFT))
            {
                aabb.SetMin(aabb.GetMin() + TsVector3::left);
                aabb.SetMax(aabb.GetMax() + TsVector3::left);
            }
            if (TsWINGetKey(VK_RIGHT))
            {
                aabb.SetMin(aabb.GetMin() + TsVector3::right);
                aabb.SetMax(aabb.GetMax() + TsVector3::right);
            }

            if (TsWINGetKey(VK_UP))
            {
                aabb.SetMin(aabb.GetMin() + TsVector3::front);
                aabb.SetMax(aabb.GetMax() + TsVector3::front);
            }
            if (TsWINGetKey(VK_DOWN))
            {
                aabb.SetMin(aabb.GetMin() + TsVector3::back);
                aabb.SetMax(aabb.GetMax() + TsVector3::back);
            }

            if (TsWINGetKey('Z'))
            {
                obbEuler.y+=4;
            }
            obb.SetCenter(obbCenter);
            obb.SetRotate(TsQuaternion::CreateByEuler(obbEuler));

            if (CollisionOBBAndAABB(obb, aabb))
            {
                aabbMesh.SetColor(255, 0, 0, 1);
                obbMesh.SetColor(1, 0, 0, 1);
            }
            else
            {
                aabbMesh.SetColor(0, 1, 0, 1);
                obbMesh.SetColor(0, 1, 0, 1);
            }

            size_t z = sizeof(TsLightSetCBuffer::LightData) / sizeof(TsF32);

            //auto pBlendState = TsResourceManager::Find<TsBlendState>("ALPHA_BLEND");
            auto pBlendState = TsResourceManager::Find<TsBlendState>( "NONE" );
            pDev->GetDC()->SetBlendState( pBlendState );
            pDev->GetDC()->ApplyBlendState();

            pLightSetCB->UpdateCBuffer(pDev->GetDC());
            pLightSetCB->ApplyCBuffer(pDev->GetDC());

            TestUpdateCamera(pCamera);
            pCamera->UpdateForCBuffer(pDev);
            pDev->GetDC()->SetCBuffer(pCamera->GetCBuffer());

            rs.CallRender(pDev->GetDC());

            pDev->Flip();
        }
    };

    return 0;
}