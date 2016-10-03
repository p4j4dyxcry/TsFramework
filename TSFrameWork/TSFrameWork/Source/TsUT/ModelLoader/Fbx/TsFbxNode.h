#pragma once

struct TS_FBX_Attribute
{
	FbxSkeleton*			pSkeleton;
	FbxMesh*				pMesh;
	FbxLight*				pLight;
	FbxCamera*				pCamera;
	FbxShape*				pShape;

	FbxNull*				pNull;
	FbxLODGroup*			pLODGroup;
	FbxLayerContainer*		pCashedEffect;
	FbxMarker*				pMarker;
	FbxNurbs*				pNurbs;
	FbxPatch*				pPathch;
	FbxCameraStereo*		pCameraStereo;
	FbxCameraSwitcher*		pCameraSwitcher;
	FbxOpticalReference*	pOpticalRef;
	FbxBoundary*			pBoundary;
	FbxSubDiv*				pSubDiv;
	FbxLine*				pLine;
	FbxNurbsCurve*			pNurbsCurve;
};

class TsFbxNode : TsFbxObject
{
	TsFbxNode( TsFbxContext* pContext );
	virtual ~TsFbxNode();
	TsBool AnalizeFbxNode( FbxNode* pFbxNode );

	TsBool IsSkeleton()const;
	TsBool IsMesh()const;
	TsBool IsShape()const;
	TsBool IsLight()const;
	TsBool IsCamera()const;
private:
	TsString m_name;
	TS_FBX_Attribute m_attribute;
	TsFbxNodeAttributeType m_attributeType;
	FbxNode*  m_fbxNode;
};