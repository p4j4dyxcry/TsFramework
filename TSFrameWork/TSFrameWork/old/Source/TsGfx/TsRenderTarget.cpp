#include "TsGfx.h"

//error macro
#define RTV_CREATE_ERR_TO_RETURN	\
	if( FAILED( hr ) )\
		{\
		TsDebugLog( "Create RenderTarget Failed \n" );\
		return TS_FALSE;\
		}


TS_STATIC_LIST_INSTANTIATE(TsRenderTarget)

TsRenderTarget::TsRenderTarget() :m_rtv( nullptr ){};
TsRenderTarget::~TsRenderTarget()
{
	UnInit();
}

TsInt2 TsRenderTarget::GetRTVSize()const
{
	return m_rtvSize;
}
void TsRenderTarget::UnInit()
{
	TsSafeRelease( m_rtv );
	TsSafeRelease( m_srv );
	TsSafeRelease( m_tex2d );
}

TsBool TsRenderTarget::Create( const TsDevice& dev ,
							   const TsInt width ,
							   const TsInt height ,
							   DXGI_FORMAT format /*= DXGI_FORMAT_R8G8B8A8_UNORM*/ )
{
	UnInit();

	ID3D11Device* pDev = dev.GetDevD3D();

	// Step 1 Create Texture
	{
		TS_TEX2D_DESC texDesc( width , height , format );

		HRESULT hr = pDev->CreateTexture2D( &texDesc , nullptr , &m_tex2d );
		RTV_CREATE_ERR_TO_RETURN;
	}
	
	// Step 2 Create Render Target View
	{
		TS_RTV_DESC desc( format );

		HRESULT hr = pDev->CreateRenderTargetView( m_tex2d , &desc , &m_rtv );
		RTV_CREATE_ERR_TO_RETURN;
	}

	// Step 3 Create Shader Resource View
	{
		TS_SRV_DESC desc( format );

		HRESULT hr = pDev->CreateShaderResourceView( m_tex2d , &desc , &m_srv );
		RTV_CREATE_ERR_TO_RETURN;
	}

	// Step 4 Create Sample State 
	// *RenderTarget ごとにサンプラを持つ必要はない。

	return TS_TRUE;
}

TsRenderTarget* TsRenderTarget::CrateScreenRTV( const TsDevice& dev )
{
	ID3D11Device * pDev = dev.GetDevD3D();
	IDXGISwapChain * pSC = dev.GetSC();

	// Swap Chain から　バックバッファへのポインタを取得
	ID3D11Texture2D* pBackBuffer = NULL;
	ID3D11RenderTargetView* pD3Drtv;
	HRESULT hr = pSC->GetBuffer( 0 , __uuidof( ID3D11Texture2D ) , ( LPVOID* )&pBackBuffer );
	RTV_CREATE_ERR_TO_RETURN;

	// バックバッファへのポインタを指定してRTVを作成
	hr = pDev->CreateRenderTargetView( pBackBuffer , NULL , &pD3Drtv );
	TsSafeRelease( pBackBuffer );

	RTV_CREATE_ERR_TO_RETURN;
	TsRenderTarget* rtv = new TsRenderTarget();
	rtv->Setname( "ScreenRenderTargetView" );
	rtv->m_rtv = pD3Drtv;
	AddObject( rtv );

	return rtv;
}