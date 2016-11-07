#pragma once

enum class TS_BLEND_MODE
{
    NONE            = 0 ,   //�u�����h����
    ALPHA_BLEND     = 1 ,   //�A���t�@�u�����h
    ADD             = 2 ,   //���Z����
    SUBTRACT        = 3 ,     //���Z����
};

static TS_BLEND_MODE ConvertBlendModeToString( const char* str )
{
    if( str == "NONE" )
        return TS_BLEND_MODE::NONE;
    if( str == "ALPHA_BLEND" )
        return TS_BLEND_MODE::ALPHA_BLEND;
    if( str == "ADD" )
        return TS_BLEND_MODE::ADD;
    if( str == "SUBTRACT" )
        return TS_BLEND_MODE::SUBTRACT;

    return TS_BLEND_MODE::NONE;
}

class TsBlendState
{
public:
    TsBlendState();
    virtual ~TsBlendState();
    TsBool CreateBlendState( TsDevice* pDev ,TS_BLEND_MODE bolenMode);
    ID3D11BlendState*  GetBlendState()const
    {
        return m_pD3DBlendState;
    }
private:
    ID3D11BlendState* m_pD3DBlendState;

};