//!*******************************************************
//!	Mesh.h
//!
//!	メッシュクラスの定義
//!
//!	著作権	2016 Yuki Tsuneyama
//!	作成日時	16.01.15

#pragma once

#include "Common.h"
#include "CTransForm.h"
#include "Vertex.h"
#include "Material.h"
#include "CMeshRenderer.h"
#include "CDirectxGraphics.h"

#include "CBone.h"

class CMeshBase
{

};

class CSingleMesh : public CMeshBase
{
public:
	CSingleMesh() :m_indexBuffer(nullptr),
				   m_indexNum(0),
				   m_meshVertex(nullptr),
				   m_vertexBuffer(nullptr),
				   m_indexList(nullptr){};
	virtual ~CSingleMesh();

	virtual void SetVertexData( std::vector<CMeshVertex>& vertexList );
	virtual void SetIndexData( std::vector<int>& indexList );
	virtual void CreateBuffer( const CDirectXGraphics& dx11 );

	CMeshVertex*  GetMeshVertex()const;
	int			  GetMeshVertexCount()const;
	UINT*		  GetIndexList()const;
	int			  GetIndexCount()const;
	ID3D11Buffer* GetVertexBuffer()const;
	ID3D11Buffer* GetIndexBuffer()const;

	void CreateTangent();

	std::string		m_name;
	CMaterialBase	m_material;
	CMatrix			m_inversMatrix;
	CBoneManager	m_boneManager;
protected:

	int*			m_indexList;

	CMeshVertex*	m_meshVertex;	
	int				m_indexNum;
	int				m_meshVertexNum;

	CMeshRenderer	m_meshRenderer;
	ID3D11Buffer*	m_vertexBuffer;
	ID3D11Buffer*	m_indexBuffer;
};

class CMeshManager
{

};


typedef CHasTransform<CSingleMesh> CSingleTransformMesh;

class CModel
{
	CSingleTransformMesh * m_mesh;
};


