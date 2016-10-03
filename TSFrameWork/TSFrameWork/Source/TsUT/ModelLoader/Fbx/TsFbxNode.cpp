#include"../../TsUT.h"
#include "TsFbxHeader.h"

TsFbxNode::TsFbxNode( TsFbxContext* pFbxContext )
	:TsFbxObject( pFbxContext ),
	m_fbxNode(nullptr)
{
	memset( &m_attribute , 0 , sizeof( TS_FBX_Attribute ) );
}

TsFbxNode::~TsFbxNode()
{

}

TsBool TsFbxNode::AnalizeFbxNode( FbxNode* pFbxNode)
{
	auto attr = pFbxNode->GetNodeAttribute();
	m_attributeType = attr->GetAttributeType();

	//dynamicCastÇÕé∏îsÇµÇΩèÍçánullÇ…Ç»ÇÈ
	m_attribute.pSkeleton	= dynamic_cast< FbxSkeleton* >( attr ); 
	m_attribute.pLight		= dynamic_cast< FbxLight* >( attr );
	m_attribute.pCamera		= dynamic_cast< FbxCamera* >(attr);
	m_attribute.pShape		= dynamic_cast< FbxShape* >( attr );
	m_attribute.pMesh		= dynamic_cast< FbxMesh* >( attr );

	m_name = pFbxNode->GetName();

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