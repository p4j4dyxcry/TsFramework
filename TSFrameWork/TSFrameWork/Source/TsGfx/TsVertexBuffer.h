//!*******************************************************
//!	TsVertexBuffer.h
//!
//!	vertex buffer.
//!
//!	© 2016 Yuki Tsuneyama
#pragma once

class TsVertexBuffer : public TsBuffer
{
public:
	TsVertexBuffer();
	TsBool SetStride(TsUint stride);
	TsBool SetOffset( TsUint offset );
	TsUint GetStride()const ;
	TsUint GetOffset()const;
private:
	TsUint m_stride;
	TsUint m_offset;
};