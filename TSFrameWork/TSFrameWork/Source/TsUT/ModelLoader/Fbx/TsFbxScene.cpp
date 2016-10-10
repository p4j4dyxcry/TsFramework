#include"../../TsUT.h"
#include "TsFbxHeader.h"

TsFbxScene::TsFbxScene(TsFbxContext * pContext,TsFbxScene* pFbxScene /* nullptr */) 
    :TsFbxObject(pContext,this),
     m_pFbxScene(nullptr),
     m_pRootNode(nullptr)
{
    m_materialCount =
    m_meshCount =
    m_nodeCount =
    m_skeletonCount = 0;
}

TsBool TsFbxScene::BindFbxScene( FbxScene * pFbxScene )
{
    if( m_pFbxScene )
        return TS_FALSE;
    m_pFbxScene = pFbxScene;

    MeshToTriangulate();

    m_materialCount = m_pFbxScene->GetMaterialCount();
    m_meshCount = m_pFbxScene->GetMemberCount<FbxMesh>();
    m_nodeCount = m_pFbxScene->GetNodeCount();
    m_skeletonCount = m_pFbxScene->GetMemberCount<FbxSkeleton>();

    for( TsInt i = 0; i < m_materialCount; ++i )
    {
        TsFbxMaterial material( m_pFbxContext ,this );
        material.AnalizeForFbxMaterial( m_pFbxScene->GetMaterial( i ) );
        m_materialList.push_back( material );
    }

    m_pRootNode = TsFbxNode::Create(m_pFbxContext, this, GetFbxRootNode());
    m_pNodeList.push_back(m_pRootNode);

    ComputeNodeTree( m_pRootNode );


    ComputeBoneIndex();


    ParseMesh();

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

// ƒ|ƒŠƒSƒ“‚ðŽOŠpŒ`‚É
TsBool TsFbxScene::MeshToTriangulate()const
{
    if( m_pFbxScene == nullptr || m_pFbxContext == nullptr)
        return TS_FALSE;
    
    FbxGeometryConverter geometryConverter( m_pFbxContext->GetFbxManager() );
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
            TsFbxBone* pBone = (TsFbxBone*)m_pNodeList[i];
            pBone->SetBoneIndex( boneIndex++ );
            pBone->ComputeBindPose();
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

TsVector<TsFbxMaterial> TsFbxScene::GetMaterialList()const
{
    return m_materialList;
}

TsVector<TsFbxBone*> TsFbxScene::GetBoneList()
{
    TsVector<TsFbxBone*> result;
    for( TsUint i = 0; i < m_pNodeList.size(); ++i )
    {
        if( m_pNodeList[i]->IsSkeleton() )
        {
            result.push_back( ( TsFbxBone* )m_pNodeList[i] );
        }
    }
    return result;
}