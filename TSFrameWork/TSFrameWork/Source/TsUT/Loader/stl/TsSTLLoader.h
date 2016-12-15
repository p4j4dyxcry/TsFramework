#pragma once

// stl face
#pragma pack(1)
struct TsStlFace
{
    TsVector3 normal;
    TsVector3 pos[3];
    TsByte    reserve[2]; //データに含まれるダミー領域
};
#pragma pack()

class TsStlLoader : public TsMeshLoader
{
public:
    TsStlLoader();
    virtual ~TsStlLoader();
    TsBool LoadFromFile(const TsString& filename, TsLoadOption& option = TsLoadOption())override;
    TsBool LoadFromMemory(void * memory, size_t sz)override;
    virtual TsInt  GetMeshNum() override;
    virtual TsInt  GetVertexSize(TsInt index) override;
    virtual void*  GetVertexBuffer(TsInt index) override;
    virtual size_t GetVertexStride() override;

    TsBool SaveBinary(const char* filename);
    TsBool SaveAscii(const char* filename);

protected:
    TsStlFace* m_faceList;
    TsUint      m_size;
    
    TsBool LoadFromAscii(const char * filename);
    TsBool LoadFromBinary(const char * filename);

    TsBool IsBinary(const char* filename, TsInt loopNum = 256);
};