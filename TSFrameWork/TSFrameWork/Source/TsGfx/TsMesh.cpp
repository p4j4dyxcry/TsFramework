#include "TsGfx.h"

TsMesh::TsMesh():
m_vertexBuffer(nullptr),
m_indexBuffer(nullptr)
{}

TsMesh::~TsMesh()
{
	TsSafeDelete( m_vertexBuffer );
	TsSafeDelete( m_indexBuffer );
}

//todo
TsBool TsMesh::CreateVertexBuffer( TsDevice * pDev, void * memory , size_t sz )
{
	( void )pDev;
	( void )memory;
	( void )sz;
	return TS_FALSE;
}

//todo
TsBool TsMesh::CreateIndexBuffer( TsDevice * pDev, void * memory , size_t sz )
{
	( void )pDev;
	( void )memory;
	( void )sz;
	return TS_FALSE;
}

TsInt TsMesh::GetVertexNum()
{
	TsInt sz = m_vertexBuffer->GetBufferSize() / m_vertexBuffer->GetStride();
	return sz;
}

TsInt TsMesh::GetIndexNum()
{
	return m_indexBuffer->GetBufferSize() / sizeof(TsInt) ;
}

TsVertexBuffer* TsMesh::GetVertexBuffer()const
{
	return m_vertexBuffer;
}
TsIndexBuffer* TsMesh::GetIndexBuffer()const
{
	return m_indexBuffer;
}