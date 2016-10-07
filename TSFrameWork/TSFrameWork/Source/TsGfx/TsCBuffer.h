//!*******************************************************
//! TsCBuffer.h
//!
//! CBufferクラスの定義
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsCBuffer : 
    public TsBuffer , 
    public TsShaderResource
{
public:
    TsCBuffer();
    virtual ~TsCBuffer();

    //=============================================
    // ! BindShaderType
    // 転送先のシェーダを設定する
    // ２つ以上のシェーダに転送する場合はORで結合する。
    // @pram type 
    // @return true is success
    void BindShaderType( TS_SHADER_TYPE type ){ m_bindShaderType = type; }

    //=============================================
    // ! GetBindShaderType
    // 転送先のシェーダを取得する
    // @return シェーダの種類。初期値はUnknown
    TS_SHADER_TYPE GetBindShaderType()const{ return m_bindShaderType; }

    virtual TsBool UpdateCBuffer( TsDeviceContext* pContext ){ return TS_TRUE;}
    virtual TsBool ApplyCBuffer( TsDeviceContext* pContext ){ return TS_TRUE; }

private:
    TS_SHADER_TYPE m_bindShaderType;
};
