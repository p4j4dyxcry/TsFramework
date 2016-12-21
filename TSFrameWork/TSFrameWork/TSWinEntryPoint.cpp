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

    TsStlLoader stlLoader;

    //stlLoader.LoadFile("f:\\yl.stl");
    //stlLoader.CreateCommonData();
    //stlLoader.SetBinarySaveFlag(TS_FALSE);
    //stlLoader.SaveFile("f:\\conver.stl");

//    dir.LookAt(TsVector3::front * 100, TsVector3::back, TsVector3::up);
    auto mat = dir.GetWorldMatrix();
    TsApplicationBase api;
    api.Initialize(hInstance, nWinMode);
    TsDevice* pDev = api.GetDevice();

    Ts3DModelBinalizer b;
    b.LoadBinaly(pDev, "cache\\PC01_Kohaku_18153157856840847600.tsm");


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

    {
        Ts3DMeshConverter pConverter;
        TsMeshObject * pMesh2 = pConverter.ConvertFromFile(pDev, "F:\\cup.STL");

        for (TsInt i = 0; i < pMesh2->GetGeometryCount(); ++i)
            queue.Add(pMesh2->GetGeometry(i));
    }

//    const TsBool useAnimation = TS_TRUE;
    const TsBool useAnimation = TS_FALSE;
//     factory.LoadModelFromFile(pDev, "Resource/fbx/Unity-Chan/unitychan.fbx","Test");
//     factory.LoadModelFromFile( pDev , "Resource/fbx/miku/miku.fbx" );
//     factory.LoadModelFromFile( pDev , "Idol.fbx","Test" );
 //    factory.LoadModelFromFile(pDev, "SD_unitychan_generic.fbx","Test");
    TsResourceManager::RegisterResource(Ts3DMeshConverter::ConvertFromFile(pDev, "SD_unitychan_generic.fbx"), "Test");
 //         factory.LoadModelFromFile(pDev, "miku.fbx","Test");
     TsMeshObject * pMesh = TsResourceManager::Find<TsMeshObject>("Test");
     TsSkeleton* pSkeleton = pMesh->GetSkeleton();
     TransformParse(pMesh->GetGeometry(0)->GetTransform()->GetRootTransform());
     TsTransformBakeAnimation* pAnim = nullptr;
     if (useAnimation)
     {
         //     auto pAnim = factory.CreateBakeAnimation( "move.fbx");
         //     auto pAnim = factory.CreateBakeAnimation( "Resource/fbx/Unity-Chan/move_unity.fbx" );
         //pAnim = factory.CreateBakeAnimation("SD_unitychan_motion_Generic.fbx");
         pAnim->BindTransform(pMesh->GetGeometry(0)->GetTransform()->GetRootTransform());

         pAnim->SetTargetSkeleton(pSkeleton);
     }

     

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
    //queue.Add(&aabbMesh);

    //queue.Add(&obbMesh);

    TsSphere3D sphere;
    sphere.SetRadius(15);
    sphere.SetCenter( TsVector3::up * 15 );
    TsColliderRenderObject sphereMesh;
    sphereMesh.CreateRenderObject(pDev, &sphere);

    //queue.Add(&sphereMesh);

    rs.SetDrawQue( &queue );

    TsCamera* pCamera = pDev->GetDC()->GetMainCamera();

//    pCamera->SetLocalRotate( TsQuaternion::AngleAxis( TsVector3::up , TsRadian( 180.0f ) ) );
    pCamera->SetLocalPosition(TsVector3(0,25,-50));
    pCamera->SetLockAt( TsVector3( TsVector3( 0 , 25 , 0 ) ) );
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

    TsCollisionOctTree octTree;
    TsVector<TsAABB3D*> aabbList;
    TsVector<TsColliderRenderObject*> renderList;
    octTree.Initalize(TsAABB3D(TsVector3(-300, -300, -300), TsVector3(300, 300, 300)), 3);

    for (TsInt i = 0; i < 1024; ++i)
    {
        TsCollisionTreeForCollider* col = TsNew(TsCollisionTreeForCollider);
        TsAABB3D * aabb = TsNew(TsAABB3D);
        aabb->SetMin(TsVector3((rand() % 600) - 300, (rand() % 600) - 300, (rand() % 600) - 300));
        aabb->SetMax(aabb->GetMin() + TsVector3(8, 8, 8));
        col->SetCollider(aabb);
        octTree.Register(*aabb, col);
        
        TsColliderRenderObject* render;
        render = TsNew(TsColliderRenderObject);
        render->CreateRenderObject(pDev, aabb);

        aabbList.push_back(aabb);
        renderList.push_back(render);
        //queue.Add(render);
    }


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
            //DWORD t = timeGetTime();
            for (TsInt i = 0; i < 1024; ++i)
            {
                auto&& list = octTree.GetCollisionList( *aabbList[i] );

                for (auto it : list)
                {

                    if (it != aabbList[i] && CollisionAABBAndAABB(*(TsAABB3D*)(it), *aabbList[i]))
                    {
                        renderList[i]->SetColor(1, 0, 0, 1);
                    }
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