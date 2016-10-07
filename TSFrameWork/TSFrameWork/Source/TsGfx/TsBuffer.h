//!*******************************************************
//! TsBuffer.h
//!
//! インデックスバッファ、頂点バッファ、Cバッファを抽象化
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsBuffer : public TsNameObject
{
public:
    TsBuffer() :m_buffer( nullptr ),m_bufferSize(0){};
    virtual ~TsBuffer(){ TsSafeRelease( m_buffer ); }

    //=============================================
    // ! GetBuffer
    // Dx11　形式でバッファポインタを取得
    // @return true is success
    ID3D11Buffer* GetBuffer()const{ return m_buffer; };

    //=============================================
    // ! Create
    // Create DirectX Device & SwapChain & DeviceContext
    // @pram buffer	デバイス作成した元になるバッファデータ
    // @return true is success
    TsBool SetD3DBufferAndSize( ID3D11Buffer* buffer ,TsUint size)
    { 
        m_buffer = buffer; 
        m_bufferSize = size;
        return TRUE; 
    };

    //! Get Size
    TsUint GetBufferSize()const{ return m_bufferSize; }

private:
    ID3D11Buffer* m_buffer;
    TsUint		  m_bufferSize;
};