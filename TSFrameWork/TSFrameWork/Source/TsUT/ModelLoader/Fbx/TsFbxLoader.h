#pragma once

class TsFbxContext;
class TsTransformBakeAnimation;
class TsFbxLoader : public TsMeshLoader
{
public:
    TsFbxLoader();
    virtual~TsFbxLoader();

    //! Load From File
    virtual TsBool LoadFromFile( const TsString& filename ,TsLoadOption& option = TsLoadOption() ) override;

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

    TsString      GetAlbedoTexturePass( TsInt index );

    TsTransForm*   GetTransform(TsInt index)const;

    TsBool IsSkinMesh( TsInt index );

    TsSkeleton* GetSkeleton()const;

    // Operation test
    TsTransformBakeAnimation* CreateAnimation( TsInt no );
private:
    TsFbxContext* m_pFbxContext;
};