//!*******************************************************
//! TsMeshLoader.h
//!
//! Mesh Loader base.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsMeshLoader : TsBinary
{
public:
    TsMeshLoader(){};
    virtual ~TsMeshLoader(){};

    //! Load From File
    virtual TsBool LoadFromFile(const TsString& filename ) = 0;

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

    //! GetIndex Buffer Size
    virtual size_t GetIndexBufferSize(TsInt index){ return 0; };

    //! GetIndex Buffer
    virtual void* GetIndexBuffer(TsInt index){ return nullptr; };

protected:

private:
};