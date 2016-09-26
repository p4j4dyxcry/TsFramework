#include "TsGfx.h"

TsDeviceContext* TsDevice::GetDC()const
{
	return m_deviceContext;
}

TsBool TsDevice::CreateDevice( HWND hWnd , TsInt width , TsInt height )
{
	//===================================
	//! Step1 
	//	Setting SwapChain
	//===================================

	//リフレッシュレートの取得
	TsUint2 refreshRato;
	TsBool result = GetRefreshRato( refreshRato , width , height );
	if( result == TS_FALSE ) return result;

	TS_SWAP_CHAIN_DESC swapChainDesc( hWnd , width , height , refreshRato );

	//===================================
	//! Step2 
	//	Create Device & Context & SwapChain
	//===================================

	// ドライバの種類の列挙
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE , D3D_DRIVER_TYPE_WARP , D3D_DRIVER_TYPE_REFERENCE ,
	};
	TsUint numDriverTypes = ARRAYSIZE( driverTypes );

	// 機能レベルの列挙
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1 , D3D_FEATURE_LEVEL_11_0 ,
		D3D_FEATURE_LEVEL_10_1 , D3D_FEATURE_LEVEL_10_0 ,
		D3D_FEATURE_LEVEL_9_3 , D3D_FEATURE_LEVEL_9_2 , D3D_FEATURE_LEVEL_9_1
	};

	TsUint numFeatureLevels = ARRAYSIZE( featureLevels );

	TsUint devCreateFlag = 0;// Device Create flag
	//	   devCreateFlag |= D3D11_CREATE_DEVICE_DEBUG;

	ID3D11DeviceContext * pDevContext;

	HRESULT hr;
	for( TsUint i = 0; i < numDriverTypes; ++i )
	{
		m_driverType = driverTypes[i];
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr ,			// ディスプレイデバイスのアダプタ（ＮＵＬＬの場合最初に見つかったアダプタ）
			m_driverType ,		// デバイスドライバのタイプ
			nullptr ,			// ソフトウェアラスタライザを使用する場合に指定する
			devCreateFlag ,		// デバイスの生成フラグ
			featureLevels ,		// 機能レベル
			numFeatureLevels ,	// 機能レベル数
			D3D11_SDK_VERSION ,	// Dx11 バージョン
			&swapChainDesc ,	// スワップチェインの設定
			&m_swapChain ,		// IDXGIDwapChainインタフェース	
			&m_device ,			// ID3D11Deviceインタフェース
			&m_featureLevel ,	// サポートされている機能レベル
			&pDevContext );		// デバイスコンテキスト
		if( SUCCEEDED( hr ) )break;
	}
	if( FAILED( hr ) ){ return TS_FALSE; }

	//! Create Context 
	m_deviceContext = TsNew( TsDeviceContext( this , pDevContext ) );
	TsRenderTarget* rtv = TsRenderTarget::CrateScreenRTV( *this );
	m_deviceContext->SetScreenRTV( rtv );

	TsDepthStencil* depthStencil;

	return TS_TRUE;
}

//=============================================
// ! GetRefreshRato
// PC モニタのリフレッシュレートを取得
// @pram _out refreshRato x = 分子　y =分母
//		 width & height 画面サイズ
// @return success is true
TsBool TsDevice::GetRefreshRato( __out TsUint2& refreshRato ,
								 const TsUint width , const TsUint height )
{
	refreshRato.x = 1;
	refreshRato.y = 60;
	//変数
	HRESULT	hr = S_OK;
	IDXGIFactory* factory;				// factory
	IDXGIAdapter* adapter;				// videocard
	IDXGIOutput* adapterOutput;			// monitor
	TsUint numModes;
	DXGI_MODE_DESC* displayModeList;

	// Create a DirectX graphics interface factory
	hr = CreateDXGIFactory( __uuidof( IDXGIFactory ) , ( void** )&factory );
	if( FAILED( hr ) ){ return TS_FALSE; }

	// use the factory to create an adpter for the primary graphics interface(video card)
	hr = factory->EnumAdapters( 0 , &adapter );
	if( FAILED( hr ) ){ return TS_FALSE; }

	// enumerrate primary adapter output(monitor)
	hr = adapter->EnumOutputs( 0 , &adapterOutput );
	if( FAILED( hr ) ){ return TS_FALSE; }

	// get the number of modes that fit the DXGI_FORMAT_R8G8B8_UNORM display format forthe adapter output(monitor)
	hr = adapterOutput->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM , DXGI_ENUM_MODES_INTERLACED , &numModes , NULL );
	if( FAILED( hr ) ){ return TS_FALSE; }

	// create alist to hold all possible display modes for this monitor/video card combination
	displayModeList = TsNew( DXGI_MODE_DESC[numModes] );
	if( !displayModeList ){ return TS_FALSE; }

	// now fill the display mode list structures
	hr = adapterOutput->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM , DXGI_ENUM_MODES_INTERLACED , &numModes , displayModeList );
	if( FAILED( hr ) ){ return TS_FALSE; }

	// now go through all the display modes and find the one that matches the screen width and height
	// when a match is found store the numerator and denominator of the refresh rate for that monitor
	for( TsUint i = 0; i < numModes; i++ ){
		if( displayModeList[i].Width == width ){
			if( displayModeList[i].Height == height ){
				refreshRato.x = displayModeList[i].RefreshRate.Numerator;
				refreshRato.y = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	TsDelete( displayModeList );
	TsSafeRelease( adapterOutput );
	TsSafeRelease( adapter );
	TsSafeRelease( factory );

	return TS_TRUE;
}