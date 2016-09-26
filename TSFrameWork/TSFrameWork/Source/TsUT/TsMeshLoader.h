//!*******************************************************
//!	TsMeshLoader.h
//!
//!	Mesh Loader base.
//!
//!	© 2016 Yuki Tsuneyama
#pragma once

class TsMeshLoader : TsBinary
{
public:
	TsMeshLoader(){};
	virtual ~TsMeshLoader(){};

	//! Load From File
	virtual TsBool LoadFromFile( TsString filename ) = 0;

	//! Load From Memory
	virtual TsBool LoadFromMemory( void* memory ,size_t sz) = 0;

	//! Get Mesh Num
	virtual TsInt  GetMeshNum() = 0;

	//! Get Vertex Size by byte
	virtual TsInt  GetVertexSize( TsInt index ) = 0;

	//! Get Vertex Buffer by byte
	virtual void*  GetVertexBuffer(TsInt index )=0;

	//! Get Vertex Stride
	virtual size_t GetVertexStride() = 0;
protected:

private:
};