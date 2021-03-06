﻿//!*******************************************************
//! TsDevice.h
//!
//! lapping dx11 device.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

//----------------------------------------------------------
// Define
//----------------------------------------------------------
class TsDevice;
class TsDeviceContext;
class TsTexture;
class TsRenderTarget;

class CBufferBase;
template<typename T>
class TsCBuffer;

class TsIndexBuffer;
class TsVertexBuffer;
class TsSamplerState;

class TsRasterizerState;
class TsDepthStencilState;


//----------------------------------------------------------
//! Deviceのインターフェイスクラス【現在未使用】
//
//  マルチプラットフォームに本格的に対応することになったら
//  使用する予定
//----------------------------------------------------------
class ITsDevice
{
public:
    //get device contetxt
    virtual TsDeviceContext* GetDC()const = 0;
    virtual ID3D11Device*    GetDevD3D()const = 0;
    virtual IDXGISwapChain*	 GetSC()const = 0;
private:
};

//----------------------------------------------------------
//! Deviceのラッピングクラス
//----------------------------------------------------------
class TsDevice : ITsDevice
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    virtual ~TsDevice();

    void UnInit();

    //=============================================
    // ! CreateDevice
    // Create DirectX Device & SwapChain & DeviceContext
    //
    // @pram    hWnd	DirectXの描画ターゲットウィンドウハンドル
    //          width & height Back Buffer Size
    // @return true is success
    TsBool CreateDevice( HWND hWnd , TsInt width , TsInt height );
    
    //=============================================
    // ! Get DeviceContext
    TsDeviceContext*    GetDC()const override;

    //=============================================
    // ! Get DirectX Device Pointer
    ID3D11Device*       GetDevD3D()const override{ return m_device; }

    //=============================================
    // ! Get DirectX Swap Chain
    IDXGISwapChain*	    GetSC()const override{ return m_swapChain; }

#if 0
    //todo
    TsTexture*          CreateTextureFromFile( TsString name );
    TsRenderTarget*     CreateRenderTarget( TsString name );

    //todo CreateShaders
    TsVertexShader*     CreateVSFromCSO( TsString filename );
    TsPixelShader*      CreatePSFromCSO( TsString filename );
    TsGeometryShader*   CreateGSFromCSO( TsString filename );
    TsHullShader*       CreateHSFromCSO( TsString filename );
    TsComputeShader*    CreateCSFromCSO( TsString filename );
    TsDomainShader*     CreateDSFromCSO( TsString filename );
#endif
    TsSamplerState* CreateSamplerState( const D3D11_SAMPLER_DESC& desc );

	CBufferBase* CreateCBuffer( __in void * pData ,
                                __in size_t size )const;

    ID3D11Buffer*	CreateBuffer( __in void * pData ,
                                  __in size_t size ,
                                  __in UINT cpuAct ,
                                  __in UINT type )const;


    TsBool Flip(TsBool vSinc = TS_TRUE);

    TsIndexBuffer* CreateIndexBuffer( void* pData , size_t size )const ;
    TsVertexBuffer* CreateVertexBuffer( void* pData , size_t size ,size_t stride,TsUint offset = 0)const ;

private:
    //=============================================
    // ! GetRefreshRato
    // PC モニタのリフレッシュレートを取得
    // @pram _out refreshRato x = 分子　y =分母
    //            上記の引数は戻り値として扱う。
    //            width & height 画面サイズ
    // @return true is success
    TsBool GetRefreshRato( __out TsUint2& refreshRato ,
                           const TsUint width , const TsUint height );

    TsBool LoadDefaultShader();

private:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    ID3D11Device*       m_device;
    IDXGISwapChain*     m_swapChain;
    D3D_DRIVER_TYPE     m_driverType;
    D3D_FEATURE_LEVEL   m_featureLevel;
    TsDeviceContext*    m_deviceContext;
};

