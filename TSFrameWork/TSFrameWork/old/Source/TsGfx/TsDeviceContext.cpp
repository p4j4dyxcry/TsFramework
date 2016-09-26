
#include "TsGfx.h"

//! Constructor
TsDeviceContext::TsDeviceContext(TsDevice* pDev, ID3D11DeviceContext* pCotext ) :m_pDeviceContext( pCotext )
{
	m_pDevice = pDev;
	m_mainDepthStencil = nullptr;
};

//! Destructor
//todo

//! clear color & depth buffer
void TsDeviceContext::Clear( TsFloat4& color /* 0x~~~~~~ */ )
{
	if( m_pDeviceContext )
	{
		//! crear rendertargets
		for( int i = 0; i < MAX_RTs; ++i )
		{
			if( m_renderTarget[i] )
				m_pDeviceContext->ClearRenderTargetView( m_renderTarget[i]->GetRTV() , &color[0] );
		}

		//! crear depth stencil view
		if(m_mainDepthStencil )
			m_pDeviceContext->ClearDepthStencilView(m_mainDepthStencil->GetDSV() , D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL , 1.0f , 0 );
	}
}

TsBool TsDeviceContext::SetTexture( TsInt slotIndex,
									const TsTexture* texture , 
									TS_SHADER_TYPE type)
{
	if( m_pDeviceContext == nullptr ) return TS_FALSE;

	//! Shader Resource View の取得
	ID3D11ShaderResourceView* srv = nullptr;
	if( texture != nullptr )
		srv = texture->GetSRV();

	// テクスチャをgpuに設定
	switch( type )
	{
		case VERTEX_SHADER:		m_pDeviceContext->VSSetShaderResources(slotIndex, 1, &srv); break;
		case PIXEL_SHADER:		m_pDeviceContext->PSSetShaderResources(slotIndex, 1, &srv); break;
		case GEOMETRY_SHADER:	m_pDeviceContext->GSSetShaderResources(slotIndex, 1, &srv); break;
		case HULL_SHADER:		m_pDeviceContext->HSSetShaderResources( slotIndex , 1 , &srv );break;
		case DOMAIN_SHADER:		m_pDeviceContext->DSSetShaderResources( slotIndex , 1 , &srv );break;
		case COMPUTE_SHADER:	m_pDeviceContext->CSSetShaderResources( slotIndex , 1 , &srv );	break;
		default:
			return TS_FALSE;
	}
	return TS_TRUE;
}

TsBool TsDeviceContext::SetShader( const TsShaderEffect* se )
{
	if( m_pDeviceContext == nullptr )
		return TS_FALSE;
	//=========================================
	//! シェーダのアクティブフラグを初期化
	//=========================================
	m_activeShaders[VS_IDX] = 
	m_activeShaders[PS_IDX] =
	m_activeShaders[GS_IDX] =
	m_activeShaders[HS_IDX] =
	m_activeShaders[DS_IDX] =
	m_activeShaders[CS_IDX] = TS_FALSE;

	// ShaderEffectがnullならすべてのシェーダをnullにする
	if( se == nullptr )
	{
		m_pDeviceContext->VSSetShader( nullptr , nullptr , 0 );
		m_pDeviceContext->PSSetShader( nullptr , nullptr , 0 );
		m_pDeviceContext->GSSetShader( nullptr , nullptr , 0 );
		m_pDeviceContext->CSSetShader( nullptr , nullptr , 0 );
		m_pDeviceContext->HSSetShader( nullptr , nullptr , 0 );
		m_pDeviceContext->DSSetShader( nullptr , nullptr , 0 );

		TsDebugLog( " Shader Effect = null\n" );
		return TS_FALSE;
	}
	//=========================================
	//! アクティブシェーダの更新
	//=========================================
	if( se->GetVertexShader() && se->GetVertexShader()->GetShader() )
		m_activeShaders[VS_IDX] = TS_FALSE;
	if( se->GetPixelShader() && se->GetPixelShader()->GetShader() )
		m_activeShaders[PS_IDX] = TS_FALSE;
	if( se->GetGeometryShader() && se->GetGeometryShader()->GetShader() )
		m_activeShaders[GS_IDX] = TS_FALSE;
	if( se->GetHullShader() && se->GetHullShader()->GetShader() )
		m_activeShaders[HS_IDX] = TS_FALSE;
	if( se->GetDomainShader() && se->GetDomainShader()->GetShader() )
		m_activeShaders[DS_IDX] = TS_FALSE;
	if( se->GetComputeShader() && se->GetComputeShader()->GetShader() )
		m_activeShaders[CS_IDX] = TS_FALSE;

	//=========================================
	// ! アクティブシェーダをコンテキストに登録
	//=========================================
	if( m_activeShaders[VS_IDX] )
		m_pDeviceContext->VSSetShader( se->GetVertexShader()->GetShader() , nullptr , 0 );
	else
		m_pDeviceContext->VSSetShader( nullptr , nullptr , 0 );
	if( m_activeShaders[PS_IDX] )
		m_pDeviceContext->PSSetShader( se->GetPixelShader()->GetShader() , nullptr , 0 );
	else
		m_pDeviceContext->PSSetShader( nullptr , nullptr , 0 );

	if( m_activeShaders[GS_IDX] )
		m_pDeviceContext->GSSetShader( se->GetGeometryShader()->GetShader() , nullptr , 0 );
	else
		m_pDeviceContext->GSSetShader( nullptr , nullptr , 0 );

	if( m_activeShaders[HS_IDX] )
		m_pDeviceContext->HSSetShader( se->GetHullShader()->GetShader() , nullptr , 0 );
	else
		m_pDeviceContext->HSSetShader( nullptr , nullptr , 0 );

	if( m_activeShaders[DS_IDX] )
		m_pDeviceContext->DSSetShader( se->GetDomainShader()->GetShader() , nullptr , 0 );
	else
		m_pDeviceContext->DSSetShader( nullptr , nullptr , 0 );

	if( m_activeShaders[CS_IDX] )
		m_pDeviceContext->CSSetShader( se->GetComputeShader()->GetShader() , nullptr , 0 );
	else
		m_pDeviceContext->CSSetShader( nullptr , nullptr , 0 );

	//=========================================
	// VertexShader Or PixelShader 
	// 設定されていないときはグラボがクラッシュする。
	//=========================================
	if( ( m_activeShaders[VS_IDX] &&
		  m_activeShaders[PS_IDX] ) == TS_FALSE )
	{
		TsDebugLog( "VertexShader OR PixelShader = null\n" );
		return TS_FALSE;
	}
	return TS_TRUE;
}

//! レンダーターゲットを設定する。(この時点ではまだデバイスには反映されない。)
TsBool TsDeviceContext::SetRT( TsInt index , TsRenderTarget* view )
{
	if( ( unsigned )index > MAX_RTs )
	{
		TsDebugLog( "Out Of Range\n" );
		return TS_FALSE;
	}
	m_renderTarget[index] = view;

	return TS_TRUE;
}

//! RenderTargetの変更を適用する
TsBool TsDeviceContext::ApplyRenderTargets()
{
	ID3D11RenderTargetView * renderTargets[MAX_RTs];
	ID3D11DepthStencilView * depth = m_mainDepthStencil->GetDSV();

	int rtNum = 0;
	for( ; rtNum < MAX_RTs; ++rtNum )
	{
		if( m_renderTarget[rtNum] == nullptr )
			break;
		else
			renderTargets[rtNum] = m_renderTarget[rtNum]->GetRTV();
	}

	if( rtNum == 0 )
	{
		TsDebugLog( "RenderTargets = null\n" );
		return TS_FALSE;
	}
	m_pDeviceContext->OMSetRenderTargets( rtNum , renderTargets , depth );
	return TS_TRUE;
}

//! 実際にスクリーンに反映されるレンダーターゲットを設定する
TsBool TsDeviceContext::SetScreenRTV( TsRenderTarget * pRtv )
{
	m_mainRenderTarget = pRtv;
	return TS_TRUE;
}