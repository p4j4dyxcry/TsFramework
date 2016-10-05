#pragma once

class TsFbxContext;

class TsFbxLoader : public TsMeshLoader
{
public:
	TsFbxLoader();
	virtual~TsFbxLoader();

	//! Load From File
	virtual TsBool LoadFromFile( const TsString& filename ) override;

	//! Load From Memory
	virtual TsBool LoadFromMemory( void* memory , size_t sz ) override;

	//! Get Mesh Num
	virtual TsInt  GetMeshNum()override;

	//! Get Vertex Size by byte
	virtual TsInt  GetVertexSize( TsInt index ) override;

	//! Get Vertex Buffer by byte
	virtual void*  GetVertexBuffer( TsInt index ) override;

	//! Get Vertex Stride
	virtual size_t GetVertexStride() override;

	virtual size_t GetIndexBufferSize(TsInt index)override;

	virtual void* GetIndexBuffer(TsInt index)override;

	TsTransForm*   GetTransform(TsInt index)const;
private:
	TsFbxContext* m_pFbxContext;
};