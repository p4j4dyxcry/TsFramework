#pragma once

struct TS_FBX_Attribute
{
    FbxSkeleton*            pSkeleton;
    FbxMesh*                pMesh;
    FbxLight*               pLight;
    FbxCamera*              pCamera;
    FbxShape*               pShape;

    FbxNull*                pNull;
    FbxLODGroup*            pLODGroup;
    FbxLayerContainer*      pCashedEffect;
    FbxMarker*              pMarker;
    FbxNurbs*               pNurbs;
    FbxPatch*               Pathch;
    FbxCameraStereo*        pCameraStereo;
    FbxCameraSwitcher*      pCameraSwitcher;
    FbxOpticalReference*    pOpticalRef;
    FbxBoundary*            pBoundary;
    FbxSubDiv*              pSubDiv;
    FbxLine*                pLine;
    FbxNurbsCurve*          pNurbsCurve;
};


class TsFbxNode : public TsFbxObject
                , public TsNameObject
{
public:
    TsFbxNode( TsFbxContext* pContext , TsFbxScene* pFbxScene );
    virtual ~TsFbxNode();
    TsBool AnalizeFbxNode( FbxNode* pFbxNode );
    FbxNode* GetFbxNode()const;

    FbxSkeleton*    AsAttributeFbxSkelton()const;
    FbxMesh*        AsAttributeFbxMesh()const;
    FbxLight*       AsAttributeFbxLight()const;
    FbxCamera*      AsAttributeFbxCamera()const;
    FbxShape*       AsAttributeShape()const;

    TsBool IsSkeleton()const;
    TsBool IsMesh()const;
    TsBool IsShape()const;
    TsBool IsLight()const;
    TsBool IsCamera()const;

    TsBool SetChild(TsFbxNode* pNode);
    TsBool SetParent( TsFbxNode* pNode );

    TsTransForm* GetTransform()const;

    TsInt GetChildCount(TsBool isSubSearch = TS_FALSE)const;
    TsFbxNode* GetParent()const;
    TsFbxNode* GetFirstChild()const;
    TsFbxNode* GetSubling()const;
    static TsFbxNode* Create( TsFbxContext*  pFbxContext,
                              TsFbxScene*    pFbxScene,
                              FbxNode*       pFbxNode);
protected:
    TS_FBX_Attribute        m_attribute;
    TsFbxNodeAttributeType  m_attributeType;
    FbxNode*                m_fbxNode;
    TsFbxNode*              m_parent;
    TsFbxNode*              m_firstChild;
    TsFbxNode*              m_subling;
    TsTransForm*            m_pTransform;
    TsTransForm             m_geometricTransform;
};