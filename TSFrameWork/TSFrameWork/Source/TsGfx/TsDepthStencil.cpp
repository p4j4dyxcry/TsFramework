#include "TsGfx.h"

// error
#define DEPTH_STENCIL_CREATE_ERR_TO_RETURN \
if( FAILED( hr ) )\
{\
	TSUT::TsLog( "Create DepthStencil Failed \n" );\
	return TS_FALSE;\
}

// Static List ‚Ìì¬
TS_INSTANTIATE_NAME_OBJ_LIST(TsDepthStencil)

//! Depth Stencil ‚Ìì¬
TsBool TsDepthStencil::Create( const TsDevice& dev ,
							  TsInt width ,
							  TsInt height ,
							  DXGI_FORMAT format /*= DXGI_FORMAT_D24_UNORM_S8_UINT */)
{
	ID3D11Device* pDev = dev.GetDevD3D();

	// step 1 create depth texture
	{
		TS_TEX2D_DESC desc( width , height , format , D3D11_BIND_DEPTH_STENCIL );
		HRESULT hr = pDev->CreateTexture2D( &desc , nullptr , &m_tex2d );
		DEPTH_STENCIL_CREATE_ERR_TO_RETURN;
	}

	// step 2 create depth stencil state
	{
		TS_DEPTH_STENCIL_DESC desc ;
		HRESULT hr = pDev->CreateDepthStencilState( &desc , &m_depthStencil );
		DEPTH_STENCIL_CREATE_ERR_TO_RETURN;
	}
	
	// step 3 create view
	{	//todo 
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory( &desc , sizeof( desc ) );
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		HRESULT hr = pDev->CreateDepthStencilView( m_tex2d , &desc , &m_depthStencilView );
	}
	return TS_TRUE;
}

// Depth Stencil View‚Ì ì¬
TsDepthStencil* TsDepthStencil::CreateDSV(	TsString name,
											const TsDevice& dev ,
											TsInt width ,
											TsInt height ,
											DXGI_FORMAT format /*= DXGI_FORMAT_D24_UNORM_S8_UINT */)
{
	TsDepthStencil * dsv = TsNew( TsDepthStencil );
	auto success = dsv->Create( dev , width , height , format );

	if( !success )
	{
		TsDelete( dsv );
		return dsv;
	}
	dsv->SetName( name );

	return dsv;
}