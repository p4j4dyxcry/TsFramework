#include "TsFbxAfx.h"
TsFbxScene::TsFbxScene(TsFbxContext * pContext,TsFbxScene* pFbxScene /* nullptr */) 
    :TsFbxObject(pContext,this),
     m_pFbxScene(nullptr),
     m_pFbxImporter(nullptr),
     m_pRootNode(nullptr)
{
    m_materialCount =
    m_meshCount =
    m_nodeCount =
    m_skeletonCount = 0;
}

TsBool TsFbxScene::LoadFromFile( const TsString& filename )
{
    // FbxからScene 情報の読み込み
    if (ImportScene(filename) == TS_FALSE)
        return TS_FALSE;

    // FbxのSceneを読みやすいようにコンバート
    ConvertScene();

    // データサイズの取得
    m_materialCount = m_pFbxScene->GetMaterialCount();
    m_meshCount = m_pFbxScene->GetMemberCount<FbxMesh>();
    m_nodeCount = m_pFbxScene->GetNodeCount();
    m_skeletonCount = m_pFbxScene->GetMemberCount<FbxSkeleton>();

    // マテリアル一覧を取得
    ParseMaterial();

    // ノード構造を解析
    ParseNodeTree();
    if( m_pFbxScene->GetCurrentAnimationStack() )
    {
        if( m_pFbxContext->GetLoadOptin().loadAnimation )
        {
            TsFbxAnimation* pAnim = TsNew( TsFbxAnimation( m_pFbxContext , this ) );
            m_pAnimationList.push_back( pAnim );
        }

    }

    // ボーンノードに対してIDを割り当てる
    if( m_pFbxContext->GetLoadOptin().loadSkeleton )
        ComputeBoneIndex();

    // メッシュノードの詳細データを解析
    if( m_pFbxContext->GetLoadOptin().loadMesh )
        ParseMesh();

    return TS_TRUE;
}

TsBool TsFbxScene::ConvertScene()
{
    // convert Fbx Scene -> Maya up
    FbxAxisSystem system(FbxAxisSystem::eMayaYUp);
    FbxAxisSystem globalSystem = m_pFbxScene->GetGlobalSettings().GetAxisSystem();

    if( globalSystem != system )
        system.ConvertScene(m_pFbxScene);

    // conver mesh to triangle
    MeshToTriangulate();

    //todo length unit convertも入れる？

    return TS_TRUE;
}

TsBool TsFbxScene::ImportScene(const TsString& filename)
{
    m_fileName = filename;
    m_pFbxImporter = FbxImporter::Create(m_pFbxContext->GetFbxManager(), filename.c_str());
    m_pFbxScene = FbxScene::Create(m_pFbxContext->GetFbxManager(), (filename + "Scene").c_str());
    TsBool hr = m_pFbxImporter->Initialize(filename.c_str());
    m_pFbxImporter->Import(m_pFbxScene);
    if (m_pFbxScene == nullptr || hr == TS_FALSE)
    {
        return TS_FALSE;
    }


    return TS_TRUE;
}

TsBool TsFbxScene::ParseNodeTree()
{
    m_pRootNode = TsFbxNode::Create(m_pFbxContext, this, GetFbxRootNode());
    m_pNodeList.push_back(m_pRootNode);

    ComputeNodeTree(m_pRootNode);

    return TS_TRUE;
}

TsBool TsFbxScene::ParseMaterial()
{
    for (TsInt i = 0; i < m_materialCount; ++i)
    {
        TsFbxMaterial material(m_pFbxContext, this);
        material.AnalizeForFbxMaterial(m_pFbxScene->GetMaterial(i));
        m_materialList.push_back(material);
    }
    return TS_TRUE;
}

TsBool TsFbxScene::ComputeNodeTree( TsFbxNode* pTsNode)
{
    FbxNode* fbxNode = pTsNode->GetFbxNode();
    for( TsInt i = 0; i < fbxNode->GetChildCount(); ++i )
    {
        auto child = fbxNode->GetChild( i );
        TsFbxNode* pTsChildNode = TsFbxNode::Create(m_pFbxContext, this, child );
        pTsChildNode->SetParent( pTsNode );
        m_pNodeList.push_back(pTsChildNode);
        ComputeNodeTree( pTsChildNode );
    }
    return TS_TRUE;
}

TsFbxScene::~TsFbxScene()
{
    FbxSafeRelease( m_pFbxScene );
}

// ポリゴンを三角形に
TsBool TsFbxScene::MeshToTriangulate()const
{
    if( m_pFbxScene == nullptr || m_pFbxContext == nullptr)
        return TS_FALSE;
    
    FbxGeometryConverter geometryConverter( m_pFbxContext->GetFbxManager() );
    if( m_pFbxContext->GetLoadOptin().loadMesh )
        geometryConverter.Triangulate( m_pFbxScene , TS_TRUE );

    return TS_TRUE;
}
FbxNode* TsFbxScene::GetFbxRootNode()const
{
    if( m_pFbxScene == nullptr )
        return nullptr;
    return m_pFbxScene->GetRootNode();
}

TsBool TsFbxScene::ComputeBoneIndex()
{
    TsInt boneIndex = 0;
    for (TsUint i = 0; i < m_pNodeList.size(); ++i)
    {
        if (m_pNodeList[i]->IsSkeleton())
        {
        //    m_pNodeList[i]->SetBoneID(boneIndex++);
        }
    }
    return TS_TRUE;
}
TsFbxNode* TsFbxScene::FindNodeByName(const TsString& name)const
{
    TS_HASH hash = TSUT::StringToHash(name);

    for (auto it : m_pNodeList)
    {
        if (it->GetHashCode() == hash)
            return it;
    }
    return nullptr;
}

TsBool TsFbxScene::ParseMesh()
{
    for (TsUint i = 0; i < m_pNodeList.size(); ++i)
    {
        if (m_pNodeList[i]->IsMesh())
        {
            TsFbxMesh* pMesh = (TsFbxMesh*)m_pNodeList[i];
            pMesh->ParseFbxMesh();
        }
    }
    return TS_TRUE;
}

TsInt   TsFbxScene::GetMeshNum()const
{
    TsInt count = 0;
    for( TsUint i = 0; i < m_pNodeList.size(); ++i )
    {
        if( m_pNodeList[i]->IsMesh() )
        {
            ++count;
        }
    }
    return count;
}
TsVector<TsFbxMesh*> TsFbxScene::GetMeshList()const
{
    TsVector<TsFbxMesh*> result;
    for( TsUint i = 0; i < m_pNodeList.size(); ++i )
    {
        if( m_pNodeList[i]->IsMesh() )
        {
            result.push_back( (TsFbxMesh*)m_pNodeList[i] );
        }
    }
    return result;
}

TsVector<TsFbxNode*> TsFbxScene::GetNodeList()const
{
    return m_pNodeList;
}

TsVector<TsFbxMaterial> TsFbxScene::GetMaterialList()const
{
    return m_materialList;
}

TsVector<TsFbxNode*> TsFbxScene::GetBoneList()
{
    TsVector<TsFbxNode*> result;
    for( TsUint i = 0; i < m_pNodeList.size(); ++i )
    {
        if( m_pNodeList[i]->IsSkeleton() )
        {
            result.push_back( m_pNodeList[i] );
        }
    }
    return result;
}

TsF32 TsFbxScene::GetFrameRate()
{
   auto timeMode = m_pFbxScene->GetGlobalSettings().GetTimeMode();

   switch(  timeMode )
   {
       case FbxTime::EMode::eFrames120:
           return 120.0f;
       case FbxTime::EMode::eFrames60:
           return 60.0f;
       case FbxTime::EMode::eFrames30:
           return 30.0f;
       case FbxTime::EMode::eFrames24:
           return 24.0f;
       default:
           return 60.0f;
   }
   return 60.0f;
}

TsInt TsFbxScene::GetMaxBoneID()const
{
    TsInt maxID = -1;
    for (TsUint i = 0; i < m_pNodeList.size(); ++i)
    {
        if (m_pNodeList[i]->IsSkeleton())
        {
            maxID = TsMax(maxID, m_pNodeList[i]->GetBoneID());
        }
    }
    return maxID;
}

TsSkeleton* TsFbxScene::CreateSkeleton()
{
    if( m_pSkeletonCash )
        return m_pSkeletonCash;
    TsSkeleton* pSkeleton = TsNew( TsSkeleton) ;

    pSkeleton->SetName( m_fileName + ":fbx_Skeleton"  );

    auto&& it = GetBoneList();

    for each( auto p in it )
    {
        if (p->GetBoneID() < 0)
        {
            TsDebugLogError("warning 不正なボーンが検出されました。処理をスキップします\n\t BoneName = \"%s\"\n\t ID =%d \n", p->GetName().c_str(),p->GetBoneID());
            continue;
        }
        pSkeleton->AddBone(
            p->GetTransform() ,
            p->GetBoneID() ,
            p->GetBindPoseMatrix());        
    };
    return m_pSkeletonCash = pSkeleton;
}