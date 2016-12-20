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
    // Fbx����Scene ���̓ǂݍ���
    ImportScene(filename);

    // Fbx��Scene��ǂ݂₷���悤�ɃR���o�[�g
    ConvertScene();

    // �f�[�^�T�C�Y�̎擾
    m_materialCount = m_pFbxScene->GetMaterialCount();
    m_meshCount = m_pFbxScene->GetMemberCount<FbxMesh>();
    m_nodeCount = m_pFbxScene->GetNodeCount();
    m_skeletonCount = m_pFbxScene->GetMemberCount<FbxSkeleton>();

    // �}�e���A���ꗗ���擾
    ParseMaterial();

    // �m�[�h�\�������
    ParseNodeTree();
    if( m_pFbxScene->GetCurrentAnimationStack() )
    {
        if( m_pFbxContext->GetLoadOptin().loadAnimation )
        {
            TsFbxAnimation* pAnim = TsNew( TsFbxAnimation( m_pFbxContext , this ) );
            m_pAnimationList.push_back( pAnim );
        }

    }

    // �{�[���m�[�h�ɑ΂���ID�����蓖�Ă�
    if( m_pFbxContext->GetLoadOptin().loadSkeleton )
        ComputeBoneIndex();

    // ���b�V���m�[�h�̏ڍ׃f�[�^�����
    if( m_pFbxContext->GetLoadOptin().loadMesh )
        ParseMesh();

    // �o�C���h�|�[�Y(�����p���s��)�̉��
    if( m_pFbxContext->GetLoadOptin().loadSkeleton )
        ParseBindPose();

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

    //todo length unit convert�������H

    return TS_TRUE;
}

TsBool TsFbxScene::ImportScene(const TsString& filename)
{
    m_fileName = filename;
    m_pFbxImporter = FbxImporter::Create(m_pFbxContext->GetFbxManager(), filename.c_str());
    m_pFbxScene = FbxScene::Create(m_pFbxContext->GetFbxManager(), (filename + "Scene").c_str());
    m_pFbxImporter->Initialize(filename.c_str());
    m_pFbxImporter->Import(m_pFbxScene);
    if (m_pFbxScene == nullptr)
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

//TsBool TsFbxScene::ParseSkeleton()
//{
//    //auto&& meshList = GetMeshList();
//    //for each( auto it in m_m )
//
//    return TS_TRUE;
//}

TsBool TsFbxScene::ParseBindPose()
{
    m_pFbxBindPoseHolder = TsNew(TsFbxBindPoseHolder(m_pFbxContext, this));
    m_pFbxBindPoseHolder->ParseBindPose(this);

    for each(auto it in m_pNodeList)
    {
        if (it->IsSkeleton())
            ((TsFbxBone*)it)->ComputeBindPose();
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

// �|���S�����O�p�`��
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
            TsFbxBone* pBone = (TsFbxBone*)m_pNodeList[i];
            pBone->SetBoneIndex( boneIndex++ );
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

TsSkeleton* TsFbxScene::CreateSkeleton()
{
    if( m_pSkeletonCash )
        return m_pSkeletonCash;
    TsSkeleton* pSkeleton = TsNew( TsSkeleton) ;

    pSkeleton->SetName( m_fileName + "_Skeleton"  );

    auto&& it = GetBoneList();

    for each( auto p in it )
    {
        pSkeleton->AddBone(
            p->GetTransform() ,
            p->GetBoneIndex() ,
            p->GetBindPoseMatrix());

    };
    return m_pSkeletonCash = pSkeleton;
}