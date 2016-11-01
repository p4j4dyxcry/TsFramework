#pragma once
class TsFbxAnimation;
class TsFbxScene :public TsFbxObject
{
public:
    TsFbxScene( TsFbxContext* pFbxContext , TsFbxScene* = nullptr);
    virtual ~TsFbxScene();
    TsBool MeshToTriangulate()const;
    FbxNode* GetFbxRootNode()const;
    TsBool	LoadFromFile( const TsString& filename );
    TsInt   GetMeshNum()const;
    TsVector<TsFbxNode*> GetNodeList()const;
    TsVector<TsFbxMesh*> GetMeshList()const;
    TsVector<TsFbxMaterial> GetMaterialList()const;
    TsFbxNode* FindNodeByName(const TsString& name)const;
    TsVector<TsFbxBone*> GetBoneList();
    TsFbxAnimation* GetAnimation( TsInt index )
    {
        return m_pAnimationList[index];
    }
    FbxScene* GetFbxScene()const
    {
        return m_pFbxScene;
    }

    FbxImporter* GetFbxImporter()const
    {
        return m_pFbxImporter;
    }
    TsMatrix* GetBindPoseMatrix(TsString& name)
    {
        return m_pFbxBindPoseHolder->GetBindPoseMatrix(name);
    }
    TsSkeleton* CreateSkeleton();
    TsF32 GetFrameRate();

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
    TsVector<TsFbxAnimation*> m_pAnimationList;
    TsFbxBindPoseHolder *   m_pFbxBindPoseHolder;

    TsString m_fileName;
private:
    TsSkeleton* m_pSkeletonCash = nullptr;
};