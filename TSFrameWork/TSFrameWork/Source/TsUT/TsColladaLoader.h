#pragma once

#include "TsXML.h"

class TsColladaLoader : public TsMeshLoader
{
public:
    TsColladaLoader();
    virtual ~TsColladaLoader();
    TsBool LoadFromFile(const TsString& filename)override;
    TsBool LoadFromMemory(void * memory, size_t sz)override;
    virtual TsInt  GetMeshNum() override;
    virtual TsInt  GetVertexSize(TsInt index) override;
    virtual void*  GetVertexBuffer(TsInt index) override;
    virtual size_t GetVertexStride() override;

    TsInt GetMaterialNum()const;
    TsString GetTexturePass(TsInt index)const;
private:
    TsXML m_xmlData;
};