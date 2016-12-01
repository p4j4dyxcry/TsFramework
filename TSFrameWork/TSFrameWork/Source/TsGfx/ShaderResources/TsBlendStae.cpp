#include "../TsGfx.h"

TsBlendState::TsBlendState()
    :m_pD3DBlendState(nullptr)
{

}

TsBlendState::~TsBlendState()
{
    TsSafeRelease( m_pD3DBlendState );
}

TsBool TsBlendState::CreateBlendState( TsDevice* pDev ,TS_BLEND_MODE blendMode)
{
    D3D11_BLEND_DESC desc;

    // zero clear
    memset( &desc , 0 , sizeof( desc ) );
    for (TsInt i = 1; i < 8; ++i)
    {
        desc.RenderTarget[i].BlendEnable = TS_FALSE;
        desc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
        desc.RenderTarget[i].DestBlend = D3D11_BLEND_ZERO;
        desc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
        desc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
        desc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }

    // �A���t�@�g�D�J�o���b�W���}���`�T���v�����O�e�N�j�b�N�Ƃ��Ďg�p���邩
    desc.AlphaToCoverageEnable  = TS_FALSE;

    // ���񏈗��̃����_�����O�����œƗ������u�����f�B���O��L�������邩
    // �����_�[�^�[�Q�b�g�P�`�V�͖��������B
    desc.IndependentBlendEnable = TS_FALSE;

    // RenderTarget 0 �̃u�����h�̗L����
    if( blendMode == TS_BLEND_MODE::NONE )
        desc.RenderTarget[0].BlendEnable = TS_FALSE;
    else
    {
        desc.RenderTarget[0].BlendEnable = TS_TRUE;
        desc.IndependentBlendEnable = TS_TRUE;
    }

    switch( blendMode )
    {
        case TS_BLEND_MODE::ALPHA_BLEND:
            desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
            desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
            desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            break;
        case TS_BLEND_MODE::ADD:
            desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
            desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
            desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            break;
        case TS_BLEND_MODE::SUBTRACT:
            desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
            desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
            desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_SUBTRACT;
            break;
        default:
            desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
            desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
            desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            break;
    }

    // �A���t�@���Src�ŏ㏑��
    desc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
    desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    desc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;

    desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    HRESULT hr;
    // create the rasterrizer state from the description we just filled out 
    hr = pDev->GetDevD3D()->CreateBlendState( &desc , &m_pD3DBlendState );
    if( FAILED( hr ) )
    {
        return TS_FALSE;
    }

    return TS_TRUE;
}