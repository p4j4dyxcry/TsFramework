//!*******************************************************
//!	TsDevice.h
//!
//!	lapping dx11 device.
//!
//!	© 2016 Yuki Tsuneyama
#pragma once

class TsDevice;
class TsDeviceContext;

// Dx11 Device Intarface
class ITsDevice
{
public:
	//get device contetxt
	virtual TsDeviceContext* GetDC()const = 0;
	virtual ID3D11Device*	 GetDevD3D()const = 0;
	virtual IDXGISwapChain*	 GetSC()const = 0;
private:
};

// Dx11 Device
class TsDevice : ITsDevice
{
public:
	//=============================================
	// ! CreateDevice
	// Create DirectX Device & SwapChain & DeviceContext
	// @pram hWnd	DirectXの描画ターゲットウィンドウハンドル
	//		 width & height Back Buffer Size
	// @return true is success
	TsBool CreateDevice( HWND hWnd , TsInt width , TsInt height );
	
	//=============================================
	// ! Get DeviceContext
	TsDeviceContext* GetDC()const override;

	//=============================================
	// ! Get DirectX Device Pointer
	ID3D11Device*	 GetDevD3D()const override{ return m_device; }

	//=============================================
	// ! Get DirectX Swap Chain
	IDXGISwapChain*	 GetSC()const override{ return m_swapChain; }
private:
	//=============================================
	// ! GetRefreshRato
	// PC モニタのリフレッシュレートを取得
	// @pram _out refreshRato x = 分子　y =分母
	//		 上記の引数は戻り値として扱う。
	//		 width & height 画面サイズ
	// @return true is success
	TsBool GetRefreshRato( __out TsUint2& refreshRato ,
						   const TsUint width , const TsUint height );
private:
	ID3D11Device*		m_device;
	IDXGISwapChain*		m_swapChain;
	D3D_DRIVER_TYPE		m_driverType;
	D3D_FEATURE_LEVEL	m_featureLevel;
	TsDeviceContext*	m_deviceContext;
};

