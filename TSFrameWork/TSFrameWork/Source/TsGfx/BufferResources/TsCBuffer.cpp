#include "../TsGfx.h"

//! Constructor
TsCBuffer::TsCBuffer()
    :m_bindShaderType(TS_SHADER_TYPE::UNKNOWN_SHADER)
{

}

//! Destructor
TsCBuffer::~TsCBuffer()
{

}

//=============================================
// ! BindShaderType
// 転送先のシェーダを設定する
// ２つ以上のシェーダに転送する場合はORで結合する。
// @pram type 
// @return true is success
void TsCBuffer::BindShaderType(TS_SHADER_TYPE type)
{ 
    m_bindShaderType = type; 
}

//=============================================
// ! GetBindShaderType
// 転送先のシェーダを取得する
// @return シェーダの種類。初期値はUnknown
TS_SHADER_TYPE TsCBuffer::GetBindShaderType()const
{ 
    return m_bindShaderType; 
}

TsBool TsCBuffer::UpdateCBuffer(TsDeviceContext* pContext)
{
    return TS_TRUE;
}
TsBool TsCBuffer::ApplyCBuffer(TsDeviceContext* pContext)
{ 
    if (pContext)
        pContext->SetCBuffer(this);
    else
        return TS_FALSE;
    return TS_TRUE; 
}