#include"../../TsUT.h"
#include "TsFbxHeader.h"

TsFbxNode::TsFbxNode( TsFbxContext* pFbxContext , TsFbxScene* pFbxScene)
:TsFbxObject(pFbxContext, pFbxScene),
    m_fbxNode(nullptr),
    m_parent(nullptr),
    m_firstChild(nullptr),
    m_subling(nullptr)
{
    memset( &m_attribute , 0 , sizeof( TS_FBX_Attribute ) );
}

TsFbxNode::~TsFbxNode()
{
}

TsBool TsFbxNode::AnalizeFbxNode( FbxNode* pFbxNode)
{
    m_fbxNode = pFbxNode;
    m_pTransform = TsNew( TsTransForm );
    FbxAMatrix localMatrix = m_fbxNode->EvaluateLocalTransform();

    *m_pTransform = FbxMatrixToTsMatrix(localMatrix);

    m_pTransform->m_localRotate.x *= -1;
    m_pTransform->m_localRotate.w *= -1;
    m_pTransform->m_localPosition.x *= -1;
    SetName( pFbxNode->GetName() );
    m_pTransform->SetName( GetName() );

    const FbxVector4 lT = pFbxNode->GetGeometricTranslation(FbxNode::eSourcePivot);
    const FbxVector4 lR = pFbxNode->GetGeometricRotation(FbxNode::eSourcePivot);
    const FbxVector4 lS = pFbxNode->GetGeometricScaling(FbxNode::eSourcePivot);

    FbxMatrix geometryMatrix = FbxMatrix(lT, lR, lS);

    m_geometricTransform = FbxMatrixToTsMatrix(geometryMatrix);

    auto attr = pFbxNode->GetNodeAttribute();
    if( attr == nullptr )
        return TS_FALSE;

    //Mayaでノードが非表示の場合はロードしない
    if (!m_fbxNode->GetVisibility())
    {
        return TS_FALSE;
    }
    //Mayaでattributeが非表示の場合はロードしない
    if (!m_fbxNode->Show.Get() )
    {
        return TS_FALSE;
    }

    m_attributeType = attr->GetAttributeType();

    switch( m_attributeType )
    {
        case TsFbxNodeAttributeType::eSkeleton:
            m_attribute.pSkeleton = static_cast< FbxSkeleton* >( attr ); break;
        case TsFbxNodeAttributeType::eLight:
            m_attribute.pLight = static_cast< FbxLight* >( attr ); break;
        case TsFbxNodeAttributeType::eCamera:
            m_attribute.pCamera = static_cast< FbxCamera* >( attr ); break;
        case TsFbxNodeAttributeType::eShape:
            m_attribute.pShape = static_cast< FbxShape* >( attr ); break;
        case TsFbxNodeAttributeType::eMesh:
            m_attribute.pMesh = static_cast< FbxMesh* >( attr ); break;
    }

    return TS_TRUE;
}
TsBool TsFbxNode::IsSkeleton()const
{
    return !!m_attribute.pSkeleton;
}
TsBool TsFbxNode::IsMesh()const
{
    return !!m_attribute.pMesh;
}
TsBool TsFbxNode::IsShape()const
{
    return !!m_attribute.pShape;
}
TsBool TsFbxNode::IsLight()const
{
    return !!m_attribute.pLight;
}
TsBool TsFbxNode::IsCamera()const
{
    return !!m_attribute.pCamera;
}

FbxNode* TsFbxNode::GetFbxNode()const
{
    return m_fbxNode;
}

FbxSkeleton*	TsFbxNode::AsAttributeFbxSkelton()const
{
    if (m_fbxNode == nullptr)
        return nullptr;
    return m_fbxNode->GetSkeleton();
}
FbxMesh*		TsFbxNode::AsAttributeFbxMesh()const
{
    if (m_fbxNode == nullptr)
        return nullptr;

    return m_fbxNode->GetMesh();
}
FbxLight*		TsFbxNode::AsAttributeFbxLight()const
{
    if (m_fbxNode == nullptr)
        return nullptr;

    return m_fbxNode->GetLight();
}
FbxCamera*		TsFbxNode::AsAttributeFbxCamera()const
{
    if (m_fbxNode == nullptr)
        return nullptr;

    return m_fbxNode->GetCamera();
}
FbxShape*		TsFbxNode::AsAttributeShape()const
{
    if (m_fbxNode == nullptr)
        return nullptr;

    return m_attribute.pShape;
}

TsBool TsFbxNode::SetChild( TsFbxNode* pNode )
{
    pNode->SetParent( this );
    return TS_TRUE;
}
TsBool TsFbxNode::SetParent( TsFbxNode* pNode )
{
    m_parent = pNode;
    m_parent->m_pTransform->AddChild( m_pTransform );
    
    if( m_parent->m_firstChild == nullptr )
        pNode->m_firstChild = this;
    else
    {
        TsFbxNode** pSubling = &pNode->m_firstChild->m_subling;
        while( *pSubling!= nullptr )
            pSubling = &(*pSubling)->m_subling;
        *pSubling = this;
    }
    return TS_TRUE;
}

TsInt TsFbxNode::GetChildCount( TsBool isSubSearch )const
{
    return m_fbxNode->GetChildCount( isSubSearch );
}

TsFbxNode* TsFbxNode::GetParent()const
{
    return m_parent;
}
TsFbxNode* TsFbxNode::GetFirstChild()const
{
    return m_firstChild;
}
TsFbxNode* TsFbxNode::GetSubling()const
{
    return m_subling;
}

TsFbxNode* TsFbxNode::Create( TsFbxContext * pFbxContext,
                              TsFbxScene   * pFbxScene,
                              FbxNode	   * pFbxNode)
{
    TsFbxNode* pTsFbxNode;

    auto attr = pFbxNode->GetNodeAttribute();
    if (attr == nullptr)
    {
        pTsFbxNode = TsNew(TsFbxNode(pFbxContext, pFbxScene));
        pTsFbxNode->AnalizeFbxNode(pFbxNode);
        return pTsFbxNode;
    }

    auto attributeType = attr->GetAttributeType();
    switch (attributeType)
    {
    case TsFbxNodeAttributeType::eSkeleton:
        pTsFbxNode = TsNew(TsFbxBone(pFbxContext, pFbxScene));
        break;
    case TsFbxNodeAttributeType::eLight:
        pTsFbxNode = TsNew(TsFbxNode(pFbxContext, pFbxScene));
        break;
    case TsFbxNodeAttributeType::eCamera:
        pTsFbxNode = TsNew(TsFbxNode(pFbxContext, pFbxScene));
        break;
    case TsFbxNodeAttributeType::eShape:
        pTsFbxNode = TsNew(TsFbxNode(pFbxContext, pFbxScene));
        break;
    case TsFbxNodeAttributeType::eMesh:
        pTsFbxNode = TsNew(TsFbxMesh(pFbxContext, pFbxScene));
        break;
    default:
        pTsFbxNode = TsNew(TsFbxNode(pFbxContext, pFbxScene));
    }
    pTsFbxNode->AnalizeFbxNode(pFbxNode);
    return pTsFbxNode;
}

TsTransForm* TsFbxNode::GetTransform()const
{
    return m_pTransform;
}