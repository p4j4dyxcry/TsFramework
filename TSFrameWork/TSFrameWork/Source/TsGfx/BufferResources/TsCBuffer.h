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
    void BindShaderType(TS_SHADER_TYPE type);

    //=============================================
    // ! GetBindShaderType
    // 転送先のシェーダを取得する
    // @return シェーダの種類。初期値はUnknown
    TS_SHADER_TYPE GetBindShaderType()const;

    virtual TsBool UpdateCBuffer(TsDeviceContext* pContext);
    virtual TsBool ApplyCBuffer(TsDeviceContext* pContext);

    template <typename T>
    TsBool CreateCBuffer(TsDevice* pDev, 
                         T* pData, 
                         TS_CBUFFER_REGISTER _register, 
                         TS_SHADER_TYPE bindShaderType = TS_SHADER_TYPE::ALL_SHADER)
    {
        if (sizeof(T) % sizeof(TsF32) != 0)
        {
            TsDebugLogError("Error ConstantBufferのサイズは16Byteアライメントで作成してください。");
            return TS_FALSE;
        }


        size_t sz = sizeof(T);

        ID3D11Buffer* buffer = pDev->CreateBuffer(pData,
            sz,
            D3D11_CPU_ACCESS_WRITE,
            D3D11_BIND_CONSTANT_BUFFER);

        if (buffer == nullptr)
            return TS_FALSE;

        SetD3DBufferAndSize(buffer, sz);
        SetRegisterIndex(_register);
        BindShaderType(bindShaderType);

        return TS_TRUE;
    }
                                                  
    template <typename T>
    TsBool ChangedCBuffer(TsDeviceContext* pDeviceContext, T* pData)
    {
        if (pDeviceContext)
        {
            pDeviceContext->ChangeCBuffer(this, pData, sizeof(T));
            return TS_TRUE;
        }
        return TS_FALSE;
    }
private:
    TS_SHADER_TYPE m_bindShaderType;
};
