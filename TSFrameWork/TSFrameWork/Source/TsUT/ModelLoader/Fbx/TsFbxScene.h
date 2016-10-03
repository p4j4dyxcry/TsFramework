#pragma once

class TsFbxScene :public TsFbxObject
{
public:
	TsFbxScene( TsFbxContext* pFbxContext );
	virtual ~TsFbxScene();
	TsBool MeshToTriangulate()const;
	FbxNode* GetFbxRootNode()const;
	TsBool	BindFbxScene( FbxScene* pFbxScene );
private:
	TsBool ComputeNodeTree( TsFbxNode* pTsNode);

	TsString	m_name;
	FbxScene*	m_pFbxScene;

	TsInt		m_meshCount;
	TsInt		m_materialCount;
	TsInt		m_nodeCount;
	TsInt		m_skeletonCount;

	TsFbxNode*	m_pRootNode;
	TsVector<TsFbxMaterial> m_materialList;
private:
};