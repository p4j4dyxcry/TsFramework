#pragma once

class TsMeshFactory;

class TsMesh :public TsNameObject
{
	friend class TsMeshFactory;

public:
	TsMesh();
	virtual ~TsMesh();
	TsBool CreateVertexBuffer( TsDevice * ,void * memory , size_t sz);
	TsBool CreateIndexBuffer( TsDevice * , void * memory , size_t sz );
	TsInt  GetVertexNum( );
	TsInt  GetIndexNum();
	TsVertexBuffer* GetVertexBuffer(  )const;
	TsIndexBuffer* GetIndexBuffer(  )const;
protected:
	TsVertexBuffer*	m_vertexBuffer;
	TsIndexBuffer*	m_indexBuffer;
};


