#pragma once

class TsFbxScene :public TsFbxObject
{
public:
    TsFbxScene( TsFbxContext* pFbxContext , TsFbxScene* = nullptr);
    virtual ~TsFbxScene();
    TsBool MeshToTriangulate()const;
    FbxNode* GetFbxRootNode()const;
    TsBool	LoadFromFile( const TsString& filename );
    TsInt   GetMeshNum()const;
    TsVector<TsFbxMesh*> GetMeshList()const;
    TsVector<TsFbxMaterial> GetMaterialList()const;
    TsFbxNode* FindNodeByName(const TsString& name)const;
    TsVector<TsFbxBone*> GetBoneList();

    FbxScene* GetFbxScene()const
    {
        return m_pFbxScene;
    }

    FbxImporter* GetFbxImporter()const
    {
        return m_pFbxImporter;
    }
    TsMatrix* GetFirstBindPoseMatrix(TsString& name)
    {
        return m_pFbxBindPoseHolder->GetFirstBindPoseMatrix(name);
    }
    TsSkeleton* CreateSkeleton();
private:
    TsBool ImportScene(const TsString& filename);
    TsBool ConvertScene();
    TsBool ComputeNodeTree( TsFbxNode* pTsNode);
    TsBool ComputeBoneIndex();
    TsBool ParseBindPose();
    TsBool ParseNodeTree();
    TsBool ParseMesh();
    TsBool ParseMaterial();

    TsString        m_name;
    FbxScene*       m_pFbxScene;
    FbxImporter*    m_pFbxImporter;

    TsInt       m_meshCount;
    TsInt       m_materialCount;
    TsInt       m_nodeCount;
    TsInt       m_skeletonCount;
    
    TsFbxNode*	m_pRootNode;
    TsVector<TsFbxNode*>    m_pNodeList;
    TsVector<TsFbxMaterial> m_materialList;
    TsFbxBindPoseHolder *   m_pFbxBindPoseHolder;
private:
    TsSkeleton* m_pSkeletonCash = nullptr;
};