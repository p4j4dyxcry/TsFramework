#include "TsGfx.h"

TsVertexElement::TsVertexElement():
m_vertexBuffer(nullptr),
m_indexBuffer(nullptr)
{}

TsVertexElement::~TsVertexElement()
{
    TsSafeDelete( m_vertexBuffer );
    TsSafeDelete( m_indexBuffer );
}

//todo
TsBool TsVertexElement::CreateVertexBuffer( TsDevice * pDev, void * memory , size_t sz )
{
    ( void )pDev;
    ( void )memory;
    ( void )sz;
    return TS_FALSE;
}

//todo
TsBool TsVertexElement::CreateIndexBuffer( TsDevice * pDev, void * memory , size_t sz )
{
    ( void )pDev;
    ( void )memory;
    ( void )sz;
    return TS_FALSE;
}

TsInt TsVertexElement::GetVertexNum()
{
    TsInt sz = m_vertexBuffer->GetBufferSize() / m_vertexBuffer->GetStride();
    return sz;
}

TsInt TsVertexElement::GetIndexNum()
{
    return m_indexBuffer->GetBufferSize() / sizeof(TsInt) ;
}

TsVertexBuffer* TsVertexElement::GetVertexBuffer()const
{
    return m_vertexBuffer;
}
TsIndexBuffer* TsVertexElement::GetIndexBuffer()const
{
    return m_indexBuffer;
}