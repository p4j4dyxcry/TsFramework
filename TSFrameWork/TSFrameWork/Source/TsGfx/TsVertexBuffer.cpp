#include "TsGfx.h"

TsVertexBuffer::TsVertexBuffer()
    :m_offset( 0 ) , m_stride( 0 )
{

}

TsBool TsVertexBuffer::SetStride( TsUint stride )
{
    m_stride = stride;
    return TS_TRUE;
}
TsBool TsVertexBuffer::SetOffset( TsUint offset )
{
    m_offset = offset;
    return TS_TRUE;
}
TsUint TsVertexBuffer::GetStride()const
{
    return m_stride;
}
TsUint TsVertexBuffer::GetOffset()const
{
    return m_offset;
}