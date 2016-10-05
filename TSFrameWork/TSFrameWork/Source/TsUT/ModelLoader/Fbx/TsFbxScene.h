#pragma once

class TsFbxScene :public TsFbxObject
{
public:
	TsFbxScene( TsFbxContext* pFbxContext , TsFbxScene* = nullptr);
	virtual ~TsFbxScene();
	TsBool MeshToTriangulate()const;
	FbxNode* GetFbxRootNode()const;
	TsBool	BindFbxScene( FbxScene* pFbxScene );
	TsInt   GetMeshNum()const;
	TsVector<TsFbxMesh*> GetMeshList()const;

	TsFbxNode* FindNodeByName(const TsString& name)const;
	TsVector<TsFbxBone*> GetBoneList();
private:
	TsBool ComputeNodeTree( TsFbxNode* pTsNode);
	TsBool ComputeBoneIndex();
	TsBool ParseMesh();
	TsString	m_name;
	FbxScene*	m_pFbxScene;

	TsInt		m_meshCount;
	TsInt		m_materialCount;
	TsInt		m_nodeCount;
	TsInt		m_skeletonCount;

	TsFbxNode*	m_pRootNode;
	TsVector<TsFbxNode*>    m_pNodeList;
	TsVector<TsFbxMaterial> m_materialList;
private:
};