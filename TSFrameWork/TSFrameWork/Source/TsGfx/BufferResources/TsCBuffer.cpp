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
// �]����̃V�F�[�_��ݒ肷��
// �Q�ȏ�̃V�F�[�_�ɓ]������ꍇ��OR�Ō�������B
// @pram type 
// @return true is success
void TsCBuffer::BindShaderType(TS_SHADER_TYPE type)
{ 
    m_bindShaderType = type; 
}

//=============================================
// ! GetBindShaderType
// �]����̃V�F�[�_���擾����
// @return �V�F�[�_�̎�ށB�����l��Unknown
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