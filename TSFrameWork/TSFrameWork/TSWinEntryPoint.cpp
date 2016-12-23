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

    if (TsWINIsLeftClick())
    {
        TsVector3 diff;
        diff.y = pos.x - old.x;
        diff.x = pos.y - old.y;


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
#include <fstream>
#include <functional>
#include <iomanip>
void TransformParse(TsTransForm* root, const char * out = "F:\\data.h")
{
    std::ofstream ofs(out);

    std::function<void(TsTransForm*)> write = [&](TsTransForm * parent)
    {
        for (auto ptr = parent; ptr->GetParent(); ptr = ptr->GetParent())
            ofs << "\t";
        ofs << parent->GetName() << std::endl;
        auto child = parent->GetFirstChild();
        auto sub = parent->GetSubling();
        if (child)
        {
            auto ptr = child;
            write(child);
        }
        if (sub)
        {
            write(sub);
        }
    };

    std::function<void(TsTransForm*)> writepos = [&](TsTransForm * parent)
    {
        ofs << parent->GetName() << std::endl;
        TsMatrix m = parent->ToLocalMatrix();
        ofs << std::setw(10) << std::fixed << std::setprecision(4) << m._11 << "::" << std::setw(10) << m._12 << "::" << std::setw(10) << m._13 << "::" << std::setw(10) << m._14 << std::endl;
        ofs << std::setw(10) << std::fixed << std::setprecision(4) << m._21 << "::" << std::setw(10) << m._22 << "::" << std::setw(10) << m._23 << "::" << std::setw(10) << m._24 << std::endl;
        ofs << std::setw(10) << std::fixed << std::setprecision(4) << m._31 << "::" << std::setw(10) << m._32 << "::" << std::setw(10) << m._33 << "::" << std::setw(10) << m._34 << std::endl;
        ofs << std::setw(10) << std::fixed << std::setprecision(4) << m._41 << "::" << std::setw(10) << m._42 << "::" << std::setw(10) << m._43 << "::" << std::setw(10) << m._44 << std::endl;
        ofs << std::endl;
        auto child = parent->GetFirstChild();
        auto sub = parent->GetSubling();
        if (child)
        {
            writepos(child);
        }
        if (sub)
        {
            writepos(sub);
        }
    };

    std::function<void(TsTransForm*)> writeSrt = [&](TsTransForm * parent)
    {
        ofs << parent->GetName() << std::endl;
        TsVector3 p = parent->m_localPosition;
        TsVector3 q = parent->m_localRotate.ToEuler();
        TsVector3 s = parent->m_localScale;
        ofs << std::setw(7) << std::fixed << std::setprecision(4) << p.x << "::" << std::setw(7) << p.y << "::" << std::setw(7) << p.z << std::endl;
        ofs << std::setw(7) << std::fixed << std::setprecision(4) << q.x << "::" << std::setw(7) << q.y << "::" << std::setw(7) << q.z << std::endl;
        ofs << std::setw(7) << std::fixed << std::setprecision(4) << s.x << "::" << std::setw(7) << s.y << "::" << std::setw(7) << s.z << std::endl;
        ofs << std::endl;
        auto child = parent->GetFirstChild();
        auto sub = parent->GetSubling();
        if (child)
        {
            writeSrt(child);
        }
        if (sub)
        {
            writeSrt(sub);
        }
    };

    write(root);
    writepos(root);
    writeSrt(root);
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

//     factory.LoadModelFromFile(pDev, "Resource/fbx/Unity-Chan/unitychan.fbx","Test");
//     factory.LoadModelFromFile( pDev , "Resource/fbx/miku/miku.fbx" );
//     factory.LoadModelFromFile( pDev , "Idol.fbx","Test" );
 //    factory.LoadModelFromFile(pDev, "SD_unitychan_generic.fbx","Test");
    TsResourceManager::RegisterResource(Ts3DMeshConverter::ConvertFromFile(pDev, "Resource/fbx/Unity-Chan/unitychan.fbx"), "Test");
 //         factory.LoadModelFromFile(pDev, "miku.fbx","Test");
     TsMeshObject * pMesh = TsResourceManager::Find<TsMeshObject>("Test");
     TransformParse(pMesh->GetGeometry(0)->GetTransform()->GetRootTransform());
     TsTransformBakeAnimation* pAnim = nullptr;

     const TsBool useAnimation = TS_TRUE;
     //    const TsBool useAnimation = TS_FALSE;
     if (useAnimation)
     {
         //     auto pAnim = factory.CreateBakeAnimation( "move.fbx");
         //     auto pAnim = factory.CreateBakeAnimation( "Resource/fbx/Unity-Chan/move_unity.fbx" );
         TsFbxLoader fbxLoader;
         fbxLoader.SetLoadGeometoryFlag( TS_FALSE );
         fbxLoader.SetLoadAnimationFlag( TS_TRUE );
         fbxLoader.LoadFile( "Resource/fbx/Unity-Chan/move_unity.fbx" );
         pAnim = fbxLoader.CreateAnimation( 0 );
         pAnim->BindTransform(pMesh->GetGeometry(0)->GetTransform()->GetRootTransform());

         TsSkeleton* pSkeleton = pMesh->GetSkeleton();
         pAnim->SetTargetSkeleton(pSkeleton);
     }

    for (TsInt i = 0; i < pMesh->GetGeometryCount(); ++ i)   
        queue.Add(pMesh->GetGeometry(i));

    TsPlaneObject plane;
    TsTransForm planeTransform;
    planeTransform.m_localScale = TsVector3( 500 , 1 , 500 );
    plane.Create( pDev );
    plane.SetTransform( &planeTransform );
    queue.Add(&plane);

    TsSphere3D sphere;
    sphere.SetRadius(15);
    sphere.SetCenter( TsVector3::up * 15 );
    TsColliderRenderObject sphereMesh;
    sphereMesh.CreateRenderObject(pDev, &sphere);

    //queue.Add(&sphereMesh);

    rs.SetDrawQue( &queue );

    TsCamera* pCamera = pDev->GetDC()->GetMainCamera();

//    pCamera->SetLocalRotate( TsQuaternion::AngleAxis( TsVector3::up , TsRadian( 180.0f ) ) );
    pCamera->SetLocalPosition(TsVector3(0,100,-500));
    pCamera->SetLockAt( TsVector3( TsVector3( 0 , 100 , 0 ) ) );
    pCamera->SetNearAndFar(4, 2000);

    pCamera->CreateCBuffer(pDev);

    TsDrawQueue postQue;
    TsScreenObject * pScreen = TsNew( TsScreenObject );
    pScreen->Create(pDev);
    postQue.Add( pScreen );

    rs.SetDrawQue( &postQue , TsRenderSystem::TARGET_FLOW::POST_RENDERER );
    MSG msg;

    TsVector3 obbCenter = TsVector3(15, 2.5f, 0);
    TsVector3 obbEuler = TsVector3(45, 45, 0);

    TsCollisionOctTree octTree;
    octTree.Initalize(TsAABB3D(TsVector3(-300, -300, -300), TsVector3(300, 300, 300)), 3);


    TsColliderRenderManager colliderRenderManager;
    colliderRenderManager.Initialize( pDev );
    queue.Add( &colliderRenderManager );
    TsVector<TsAABB3D*> aabbList;
    aabbList.reserve( 1024 );

    TsAABB3D aabb;
    for (TsInt i = 0; i < pMesh->GetGeometryCount(); ++i)
    {
        aabb.MergeAABB( pMesh->GetGeometry( i )->GetAABB() );
//        aabbList.push_back( aabb );
    }
    TsAABB3D scene = TsAABB3D( TsVector3( -250 , -10 , -250 ) , TsVector3( 250 , 300 , 250 ) );
    colliderRenderManager.AddGeometory( &aabb );
    colliderRenderManager.AddGeometory( &scene );



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
            if ( pAnim ) 
                pAnim->Update();

            for( auto aabb : aabbList )
            {
                if( rand() % 30 == 0 )
                {
                    aabb->SetMin( TsVector3( ( rand() % 1500 ) - 750 , ( rand() % 1500 ) - 750 , ( rand() % 1500 ) - 750 ) );
                    aabb->SetMax( aabb->GetMin() + TsVector3( rand() % 50 + 1 , rand() % 50 + 1 , rand() % 50 + 1 ) );
                }
            }
                     
            size_t z = sizeof(TsLightSetCBuffer::LightData) / sizeof(TsF32);

            auto pBlendState = TsResourceManager::Find<TsBlendState>("ALPHA_BLEND");
            //auto pBlendState = TsResourceManager::Find<TsBlendState>( "NONE" );
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