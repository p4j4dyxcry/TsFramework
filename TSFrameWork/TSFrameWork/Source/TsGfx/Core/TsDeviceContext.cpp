
#include "../TsGfx.h"

//! Constructor
TsDeviceContext::TsDeviceContext(TsDevice* pDev, ID3D11DeviceContext* pCotext ) 
    :m_pDeviceContext( pCotext ),
    m_bindShaderEffect(nullptr),
    m_mainCamera(nullptr)
{
    m_pDevice = pDev;
    m_mainDepthStencil = nullptr;
    m_pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
};

//! Destructor
TsDeviceContext::~TsDeviceContext()
{
    TsSafeDelete( m_mainRenderTarget );
    TsSafeDelete( m_mainDepthStencil );
    TsSafeRelease( m_pDeviceContext );
}

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
        if( m_depthStencil )
            m_pDeviceContext->ClearDepthStencilView(m_depthStencil->GetDSV() , D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL , 1.0f , 0 );
    }
}

//! Set texture
TsBool TsDeviceContext::SetTexture( TsInt slotIndex,
                                    const TsTexture* texture , 
                                    TS_SHADER_TYPE type)
{
    if( m_pDeviceContext == nullptr ) return TS_FALSE;

    //! Shader Resource View の取得
    ID3D11ShaderResourceView* srv = nullptr;
    if( texture != nullptr )
        srv = texture->GetSRV();
    else
        return TS_FALSE;

    // テクスチャをgpuに設定
    switch( type )
    {
        case VERTEX_SHADER:		m_pDeviceContext->VSSetShaderResources( slotIndex , 1 , &srv ); break;
        case PIXEL_SHADER:		m_pDeviceContext->PSSetShaderResources( slotIndex , 1 , &srv ); break;
        case GEOMETRY_SHADER:	m_pDeviceContext->GSSetShaderResources( slotIndex , 1 , &srv ); break;
        case HULL_SHADER:		m_pDeviceContext->HSSetShaderResources( slotIndex , 1 , &srv ); break;
        case DOMAIN_SHADER:		m_pDeviceContext->DSSetShaderResources( slotIndex , 1 , &srv ); break;
        case COMPUTE_SHADER:	m_pDeviceContext->CSSetShaderResources( slotIndex , 1 , &srv );	break;
        default:
            return TS_FALSE;
    }
    return TS_TRUE;
}

TsBool TsDeviceContext::SetSamplerState( TsSamplerState * pSamler ,
                                         TsInt registerIndex ,
                                         TS_SHADER_TYPE ShaderType )
{
    if( m_pDeviceContext == nullptr ) return TS_FALSE;

    //! Shader Resource View の取得
    ID3D11SamplerState* pD3DSampler = nullptr;
    if( pSamler != nullptr )
        pD3DSampler = pSamler->GetSamplerState();
    else
        return TS_FALSE;

    // テクスチャをgpuに設定
    switch( ShaderType )
    {
        case VERTEX_SHADER:		m_pDeviceContext->VSSetSamplers( registerIndex , 1 , &pD3DSampler ); break;
        case PIXEL_SHADER:		m_pDeviceContext->PSSetSamplers( registerIndex , 1 , &pD3DSampler ); break;
        case GEOMETRY_SHADER:	m_pDeviceContext->GSSetSamplers( registerIndex , 1 , &pD3DSampler ); break;
        case HULL_SHADER:		m_pDeviceContext->HSSetSamplers( registerIndex , 1 , &pD3DSampler ); break;
        case DOMAIN_SHADER:		m_pDeviceContext->DSSetSamplers( registerIndex , 1 , &pD3DSampler ); break;
        case COMPUTE_SHADER:	m_pDeviceContext->CSSetSamplers( registerIndex , 1 , &pD3DSampler ); break;
        default:
            return TS_FALSE;
    }
    return TS_TRUE;
}

//! Set Shader pipline
TsBool TsDeviceContext::SetShader( TsShaderEffect* se )
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
        m_activeShaders[VS_IDX] = TS_TRUE;
    if( se->GetPixelShader() && se->GetPixelShader()->GetShader() )
        m_activeShaders[PS_IDX] = TS_TRUE;
    if( se->GetGeometryShader() && se->GetGeometryShader()->GetShader() )
        m_activeShaders[GS_IDX] = TS_TRUE;
    if( se->GetHullShader() && se->GetHullShader()->GetShader() )
        m_activeShaders[HS_IDX] = TS_TRUE;
    if( se->GetDomainShader() && se->GetDomainShader()->GetShader() )
        m_activeShaders[DS_IDX] = TS_TRUE;
    if( se->GetComputeShader() && se->GetComputeShader()->GetShader() )
        m_activeShaders[CS_IDX] = TS_TRUE;

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
    m_bindShaderEffect = se;
    return TS_TRUE;
}

//! レンダーターゲットを設定する。(この時点ではまだデバイスには反映されない。)
TsBool TsDeviceContext::SetRT( TsInt index , TsRenderTarget* view )
{
    if( m_pDeviceContext == nullptr )
        return TS_FALSE;

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
    if( m_pDeviceContext == nullptr )
        return TS_FALSE;

    ID3D11RenderTargetView * renderTargets[MAX_RTs];
    ID3D11DepthStencilView * dsv = nullptr;

    if( m_depthStencil != nullptr )
    {
        dsv = m_depthStencil->GetDSV();
    }

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

    m_pDeviceContext->OMSetRenderTargets( rtNum , renderTargets , dsv );

    return TS_TRUE;
}

//! 実際にスクリーンに反映されるレンダーターゲットを設定する
TsBool TsDeviceContext::SetScreenRTV( TsRenderTarget * pRtv )
{
    if( m_pDeviceContext == nullptr )
        return TS_FALSE;

    m_mainRenderTarget = pRtv;
    return TS_TRUE;
}


//! Set Depth Stencil
TsBool TsDeviceContext::SetDepthStencilView(TsDepthStencilView* dsv)
{
    if( m_pDeviceContext == nullptr )
        return TS_FALSE;
    if( dsv )
        m_depthStencil = dsv;
    return TS_TRUE;
}

//! Set Depth Stencil
TsBool TsDeviceContext::SetMainDepthStencil( TsDepthStencilView* dsv )
{
    if( m_pDeviceContext == nullptr )
        return TS_FALSE;

    m_mainDepthStencil = dsv;
    return TS_TRUE;
}

// ! Set & Change the CBuffer.
TsBool TsDeviceContext::SetAndChangeCBuffer( TsCBuffer* cbuffer ,
                                             void * pData ,
                                             size_t size )
{
    if( m_pDeviceContext == nullptr )
        return TS_FALSE;

    TsBool hr;
    hr = ChangeCBuffer( cbuffer , pData , size );
    if( hr == TS_FALSE )return TS_FALSE;

    hr = SetCBuffer( cbuffer );
    if( hr == TS_FALSE ) return TS_FALSE;

    return TS_TRUE;
}

//! Set CBuffer
TsBool TsDeviceContext::SetCBuffer(const TsCBuffer * cbuffer )
{
    if( m_pDeviceContext == nullptr )
        return TS_FALSE;

    if( cbuffer == nullptr )
        return TS_FALSE;

    ID3D11Buffer* buffer = cbuffer->GetBuffer();

    auto shaderType = cbuffer->GetBindShaderType();

    if( buffer == nullptr )
        return TS_FALSE;

    if(TSShaderUT::IsVS( shaderType ))
        m_pDeviceContext->VSSetConstantBuffers( cbuffer->GetRegisterIndex() , 1 , &buffer );
    if(TSShaderUT::IsPS( shaderType ) )
        m_pDeviceContext->PSSetConstantBuffers( cbuffer->GetRegisterIndex() , 1 , &buffer );
    if(TSShaderUT::IsGS( shaderType ) )
        m_pDeviceContext->CSSetConstantBuffers( cbuffer->GetRegisterIndex() , 1 , &buffer );
    if(TSShaderUT::IsHS( shaderType ) )
        m_pDeviceContext->HSSetConstantBuffers( cbuffer->GetRegisterIndex() , 1 , &buffer );
    if(TSShaderUT::IsCS( shaderType ) )
        m_pDeviceContext->CSSetConstantBuffers( cbuffer->GetRegisterIndex() , 1 , &buffer );
    if(TSShaderUT::IsDS( shaderType ) )
        m_pDeviceContext->DSSetConstantBuffers( cbuffer->GetRegisterIndex() , 1 , &buffer );

    if( shaderType == TS_SHADER_TYPE::UNKNOWN_SHADER )
    {
        TsDebugLog( "Set CBuffer Error.\n" );
        return TS_FALSE;
    }
    return TS_TRUE;
}

//! Change CBuffer
TsBool TsDeviceContext::ChangeCBuffer( TsCBuffer * cbuffer , void * pData , size_t sz )
{
    if( m_pDeviceContext == nullptr )
        return TS_FALSE;

    if( cbuffer == nullptr )
        return TS_FALSE;

    ID3D11Buffer* buffer = cbuffer->GetBuffer();

    if( buffer == nullptr )
        return TS_FALSE;

    D3D11_MAPPED_SUBRESOURCE mappedResource;

    HRESULT hr =m_pDeviceContext->Map( buffer , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &mappedResource );
    if( hr == S_OK )
    {
        CopyMemory( mappedResource.pData , pData , sz );

        m_pDeviceContext->Unmap( buffer , 0 );
    }
    else
    {
        TsDebugLog( "Gpuリソースへの書き込みに失敗\n" );
        return TS_FALSE;
    }

    return TS_TRUE;
}

//! Draw Call
TsBool TsDeviceContext::Draw( TsInt vtxNum , TsInt startSlot )
{
    if( m_pDeviceContext == nullptr )
        return TS_FALSE;

    TsBool isActiveVSandPS = m_activeShaders[VS_IDX] & m_activeShaders[PS_IDX];

    if( isActiveVSandPS )
    {
        m_pDeviceContext->Draw( vtxNum , startSlot );
        m_drawCallCount++;
    }
    else
    {
        TsDebugLog( "Fail Draw Call Vertex Or PixelShader Is Null\n" );
        return TS_FALSE;
    }
    return TS_TRUE;
}

TsBool TsDeviceContext::DrawIndex( TsInt index , TsInt startSlot  , TsInt indexLocation  )
{
    if( m_pDeviceContext == nullptr )
        return TS_FALSE;

    TsBool isActiveVSandPS = m_activeShaders[VS_IDX] & m_activeShaders[PS_IDX];

    if( isActiveVSandPS )
    {
        m_pDeviceContext->DrawIndexed( index , startSlot,indexLocation );
        m_drawCallCount++;
    }
    else
    {
        TsDebugLog( "Fail Draw Call Vertex Or PixelShader Is Null\n" );
        return TS_FALSE;
    }
    return TS_TRUE;
}

TsBool TsDeviceContext::DrawInstance( TsInt vertexNum ,
                                      TsInt instanceNum ,
                                      TsInt instanceLocation ,
                                      TsInt startSlot )
{
    if( m_pDeviceContext == nullptr )
        return TS_FALSE;

    TsBool isActiveVSandPS = m_activeShaders[VS_IDX] & m_activeShaders[PS_IDX];

    if( isActiveVSandPS )
    {
        m_pDeviceContext->DrawInstanced( vertexNum , instanceNum , startSlot , instanceLocation );
        m_drawCallCount++;
    }
    else
    {
        TsDebugLog( "Fail Draw Call Vertex Or PixelShader Is Null\n" );
        return TS_FALSE;
    }
    return TS_TRUE;
}

TsBool TsDeviceContext::DrawIndexInstance( TsInt index , 
                                           TsInt instanceNum ,
                                           TsInt instanceLocation,
                                           TsInt startSlot , 
                                           TsInt indexLocation )
{
    if( m_pDeviceContext == nullptr )
        return TS_FALSE;

    TsBool isActiveVSandPS = m_activeShaders[VS_IDX] & m_activeShaders[PS_IDX];

    if( isActiveVSandPS )
    {
        m_pDeviceContext->DrawIndexedInstanced( index , instanceNum , startSlot , indexLocation , instanceLocation );
        m_drawCallCount++;
    }
    else
    {
        TsDebugLog( "Fail Draw Call Vertex Or PixelShader Is Null\n" );
        return TS_FALSE;
    }
    return TS_TRUE;
}

TsBool TsDeviceContext::ResetDrawCallCount()
{
    m_drawCallCount = 0;
    return TS_TRUE;
}

//! Set View port
TsBool TsDeviceContext::SetViewport( TsViewport* viewport )
{
    if( m_pDeviceContext == nullptr )
        return TS_FALSE;

    if( viewport == nullptr )
        return TS_FALSE;
    m_pDeviceContext->RSSetViewports( 1 , viewport->GetD3DViewport() );

    return TS_TRUE;
}

//! Apply Inputlayout
TsBool TsDeviceContext::ApplyInputLayout()
{
    if( m_pDeviceContext == nullptr )
        return TS_FALSE;

    if( m_activeShaders[VS_IDX] == TS_FALSE )
        return TS_FALSE;

    if( m_bindShaderEffect == nullptr )
        return TS_FALSE;

    auto inputLayout = m_bindShaderEffect->GetInputLayout();
    m_pDeviceContext->IASetInputLayout( inputLayout );

    return TS_TRUE;
}

//! SetVertexBuffer
TsBool TsDeviceContext::SetVertexBuffer( TsVertexBuffer* vb )
{
    if( m_pDeviceContext == nullptr )
        return TS_FALSE;

    if( vb == nullptr )
    {
        TSUT::TsLog( "VertexBuffer is null\n" );
        return TS_FALSE;
    }

    ID3D11Buffer* buffer = vb->GetBuffer();
    TsUint size = vb->GetBufferSize();
    TsUint stride = vb->GetStride();
    TsUint offset = vb->GetOffset();
    if( buffer == nullptr || size == 0 || stride == 0)
    {
        TSUT::TsLog( "VertexBuffer is null\n" );
        return TS_FALSE;
    }

    m_pDeviceContext->IASetVertexBuffers( 0 , 1 , &buffer , &stride , &offset );

    return TS_TRUE;
}

//! SetIndexBuffer
TsBool TsDeviceContext::SetIndexBuffer( TsIndexBuffer* pBuffer)
{
    m_pDeviceContext->IASetIndexBuffer(pBuffer->GetBuffer(), DXGI_FORMAT_R32_UINT, 0);
    return TS_TRUE;
}

TsBool TsDeviceContext::SetRasterizerState( TsRasterizerState* raster )
{
    if( m_pDeviceContext == nullptr )
        return TS_FALSE;

    m_pRasterizerState = raster;

    return TS_TRUE;
}

TsBool TsDeviceContext::SetBlendState( TsBlendState* pBlendState )
{
    if( m_pDeviceContext == nullptr )
        return TS_FALSE;

    m_pBlendState = pBlendState;

    return TS_TRUE;
}

TsBool TsDeviceContext::SetDepthStencilState( TsDepthStencilState*  pDepthStencil)
{
    if( m_pDeviceContext == nullptr )
        return TS_FALSE;

    m_pDepthStencilState = pDepthStencil;
    return TS_TRUE;
}

TsCamera * TsDeviceContext::GetMainCamera()const
{
    //todo test
    if( m_mainCamera == nullptr )
    {
        static TsCamera defaultCamera;
        return &defaultCamera;
    }
    return m_mainCamera;
}

TsBool TsDeviceContext::SetMainCamera( TsCamera* camera )
{
    m_mainCamera = camera;
    return TS_TRUE;
}
TsBool TsDeviceContext::SetTopology( D3D_PRIMITIVE_TOPOLOGY topology )
{
    m_pDeviceContext->IASetPrimitiveTopology( topology );
    return TS_TRUE;
}


TsBool TsDeviceContext::ApplyDepthStencil()
{
    if( m_pDeviceContext )
        m_pDeviceContext->OMSetDepthStencilState( m_pDepthStencilState->GetDSS() , 0 );
    return TS_TRUE;
}
TsBool TsDeviceContext::ApplyRasterizer()
{
    if( m_pDeviceContext )
        m_pDeviceContext->RSSetState( m_pRasterizerState->GetRSS() );
    return TS_TRUE;
}
TsBool TsDeviceContext::ApplyBlendState()
{
    if( m_pDeviceContext )
    {
        float blendFactor[4] = { 0.0f , 0.0f , 0.0f , 0.0f };
        m_pDeviceContext->OMSetBlendState( m_pBlendState->GetBlendState() , blendFactor , 0xffffff );
    }
    return TS_TRUE;
}

TsBool TsDeviceContext::ClearVertexBuffer()
{
    return TS_FALSE;
}
TsBool TsDeviceContext::ClearIndexBuffer()
{
    return TS_FALSE;
}
TsBool TsDeviceContext::ClearCBuffer( TsInt index , TS_SHADER_TYPE type )
{ 
    TsInt count = 1;
    if( index < 0 )
    {
        index = 0;
        count = 16;
    }
    if( index > 15 )
    {
        return TS_FALSE;
    }

    if( type & TS_SHADER_TYPE::VERTEX_SHADER & type )
        m_pDeviceContext->VSSetConstantBuffers( index , count , nullptr );
    if( type & TS_SHADER_TYPE::PIXEL_SHADER & type )
        m_pDeviceContext->PSSetConstantBuffers( index , count , nullptr );
    if( type & TS_SHADER_TYPE::GEOMETRY_SHADER & type )
        m_pDeviceContext->GSSetConstantBuffers( index , count , nullptr );
    if( type & TS_SHADER_TYPE::HULL_SHADER & type )
        m_pDeviceContext->HSSetConstantBuffers( index , count , nullptr );
    if( type & TS_SHADER_TYPE::DOMAIN_SHADER & type )
        m_pDeviceContext->DSSetConstantBuffers( index , count , nullptr );
    if( type & TS_SHADER_TYPE::COMPUTE_SHADER & type )
        m_pDeviceContext->CSSetConstantBuffers( index , count , nullptr );

    return TS_TRUE;
}