#pragma once

class TsMeshFactory;

class TsMesh
{
	friend class TsMeshFactory;

public:
	TsMesh();
	virtual ~TsMesh();
	TsBool CreateVertexBuffer( TsDevice * ,void * memory , size_t sz);
	TsBool CreateIndexBuffer( TsDevice * , void * memory , size_t sz );
	TsInt  GetVertexNum( );
	TsVertexBuffer* GetVertexBuffer(  )const;
	TsIndexBuffer* GetIndexBuffer(  )const;
protected:
	TsVertexBuffer*	m_vertexBuffer;
	TsIndexBuffer*	m_indexBuffer;
};

