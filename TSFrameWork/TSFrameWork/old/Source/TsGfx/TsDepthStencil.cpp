#include "TsGfx.h"

#define DEPTH_STENCIL_CREATE_ERR_TO_RETURN \
if( FAILED( hr ) )\
{\
	TSUT::TsLog( "Create DepthStencil Failed \n" );\
	return TS_FALSE;\
}

TsBool TsDepthStencil::Create( const ITsDevice& dev ,
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
	//todo 
	// depthstencilview ‰Šú‰»
	return TS_TRUE;
}
