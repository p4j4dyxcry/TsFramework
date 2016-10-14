#pragma once

class TsMeshFactory;

class TsVertexElement :public TsNameObject
{
	friend class TsMeshFactory;

public:
	TsVertexElement();
	virtual ~TsVertexElement();
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


