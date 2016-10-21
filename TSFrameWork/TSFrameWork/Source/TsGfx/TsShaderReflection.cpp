#include "TsGfx.h"

TsShaderReflection::TsShaderReflection(ID3D11ShaderReflection* pDxShaderReflection)
{
    if (pDxShaderReflection)
        Analize(pDxShaderReflection);
}

TsBool TsShaderReflection::Analize(ID3D11ShaderReflection* pDxShaderReflection)
{
    D3D11_SHADER_DESC desc;
    pDxShaderReflection->GetDesc(&desc);

    // cbuffer count
    TsInt cbCount = desc.ConstantBuffers;
    m_constantBuffers.resize( cbCount );

    for (TsInt i = 0; i < cbCount; ++i)
    {
        m_constantBuffers[i].Analize(pDxShaderReflection->GetConstantBufferByIndex(i));
    }
    return TS_TRUE;
}

//----------------------------------------------------------------------------------
//
// ! CBuffer Reflection
//
//----------------------------------------------------------------------------------
TsBool TsShaderReflection::TsCBufferReflection::Analize(
    ID3D11ShaderReflectionConstantBuffer* pDxCbufferReflection)
{
    if (pDxCbufferReflection == nullptr)
        return TS_FALSE;

    D3D11_SHADER_BUFFER_DESC desc;
    pDxCbufferReflection->GetDesc( &desc );

    SetName(desc.Name);

    return TS_TRUE;

}

TsBool TsShaderReflection::TsCBufferReflection::SetRegsiterID(TsInt id)
{
    // cbuffer range is 0 - 15
    if (id < 0 || id > 15)
    {
        return TS_FALSE;
    }

    m_registerID = id;
    return TS_TRUE;
}
