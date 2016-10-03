#include"../../TsUT.h"
#include "TsFbxHeader.h"

TsFbxScene::TsFbxScene(TsFbxContext * pContext) 
	:TsFbxObject(pContext),
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
		TsFbxMaterial material( m_pFbxContext );
		material.AnalizeForFbxMaterial( m_pFbxScene->GetMaterial( i ) );
		m_materialList.push_back( material );
	}

	m_pRootNode = TsNew( TsFbxNode( m_pFbxContext ) );
	m_pRootNode->AnalizeFbxNode( GetFbxRootNode() );
	ComputeNodeTree( m_pRootNode );



	return TS_TRUE;
}

TsBool TsFbxScene::ComputeNodeTree( TsFbxNode* pTsNode)
{
	FbxNode* fbxNode = pTsNode->GetFbxNode();
	for( TsInt i = 0; i < fbxNode->GetChildCount(); ++i )
	{
		auto child = fbxNode->GetChild( i );
		TsFbxNode* pTsChildNode = TsNew( TsFbxNode( m_pFbxContext ) );
		pTsChildNode->AnalizeFbxNode( child );
		pTsChildNode->SetParent( pTsNode );
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