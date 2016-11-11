//**********************************************************
//! TsBlendState.h
//! BlendState.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

//----------------------------------------------------------
// ! Blend Mode Define
//----------------------------------------------------------
enum class TS_BLEND_MODE
{
    NONE            = 0 ,   //ブレンド無し
    ALPHA_BLEND     = 1 ,   //アルファブレンド
    ADD             = 2 ,   //加算合成
    SUBTRACT        = 3 ,     //減算合成
};


//----------------------------------------------------------
// ! Convert Blend Mode from String
//----------------------------------------------------------
static TS_BLEND_MODE ConvertBlendModeFromString(const char* str)
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

//----------------------------------------------------------
// ! Blend State
//----------------------------------------------------------
class TsBlendState
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsBlendState();
    virtual ~TsBlendState();
    TsBool CreateBlendState( TsDevice* pDev ,TS_BLEND_MODE bolenMode);
    ID3D11BlendState*  GetBlendState()const
    {
        return m_pD3DBlendState;
    }
private:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    ID3D11BlendState* m_pD3DBlendState;

};