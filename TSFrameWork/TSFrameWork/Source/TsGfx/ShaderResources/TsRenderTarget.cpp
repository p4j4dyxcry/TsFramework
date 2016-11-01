#include "../TsGfx.h"

//error macro
#define RTV_CREATE_ERR_TO_RETURN	\
    if( FAILED( hr ) )\
        {\
        TsDebugLog( "Create RenderTarget Failed \n" );\
        return TS_FALSE;\
        }

TS_INSTANTIATE_NAME_OBJ_LIST(TsRenderTarget)

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
        texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        HRESULT hr = pDev->CreateTexture2D( &texDesc , nullptr , &m_tex2d );
        RTV_CREATE_ERR_TO_RETURN;
    }
    
    // Step 2 Create Render Target View
    {
        HRESULT hr = pDev->CreateRenderTargetView( m_tex2d , nullptr , &m_rtv );
        RTV_CREATE_ERR_TO_RETURN;
    }
    // Step 3 Create Shader Resource View
    {
        HRESULT hr = pDev->CreateShaderResourceView( m_tex2d , nullptr , &m_srv );
        RTV_CREATE_ERR_TO_RETURN;
    }

    m_rtvSize = TsInt2( width , height );

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
    D3D11_TEXTURE2D_DESC desc;
    pBackBuffer->GetDesc( &desc );

    TsRenderTarget* rtv = new TsRenderTarget();
    rtv->SetName( "ScreenRenderTargetView" );
    rtv->m_rtv = pD3Drtv;
    rtv->m_rtvSize.x = desc.Width;
    rtv->m_rtvSize.y = desc.Height;

    TsSafeRelease( pBackBuffer );
    RTV_CREATE_ERR_TO_RETURN;
    return rtv;
}

TsRenderTarget* TsRenderTarget::CreateRTV( TsString name ,
                                           const TsDevice& dev ,
                                           const TsInt width ,
                                           const TsInt height ,
                                           DXGI_FORMAT format/* = DXGI_FORMAT_R8G8B8A8_UNORM */ )
{
    TsRenderTarget * rtv = TsNew( TsRenderTarget );
    TsBool succed = rtv->Create( dev , width , height , format );
    rtv->SetName( name );

    if( !succed )
    {
        TsSafeDelete( rtv );
        return nullptr;
    }
    return rtv;
}
