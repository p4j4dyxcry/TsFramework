#include "TsGfx.h"

TsDevice::~TsDevice()
{
	UnInit();
}

void TsDevice::UnInit()
{
	TsSafeDelete( m_deviceContext );
	TsSafeRelease( m_swapChain );
	TsSafeRelease( m_device );
}

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

	//���t���b�V�����[�g�̎擾
	TsUint2 refreshRato;
	TsBool result = GetRefreshRato( refreshRato , width , height );
	if( result == TS_FALSE ) return result;

	TS_SWAP_CHAIN_DESC swapChainDesc( hWnd , width , height , refreshRato );

	//===================================
	//! Step2 
	//	Create Device & Context & SwapChain
	//===================================

	// �h���C�o�̎�ނ̗�
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE , D3D_DRIVER_TYPE_WARP , D3D_DRIVER_TYPE_REFERENCE ,
	};
	TsUint numDriverTypes = ARRAYSIZE( driverTypes );

	// �@�\���x���̗�
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
			nullptr ,			// �f�B�X�v���C�f�o�C�X�̃A�_�v�^�i�m�t�k�k�̏ꍇ�ŏ��Ɍ��������A�_�v�^�j
			m_driverType ,		// �f�o�C�X�h���C�o�̃^�C�v
			nullptr ,			// �\�t�g�E�F�A���X�^���C�U���g�p����ꍇ�Ɏw�肷��
			devCreateFlag ,		// �f�o�C�X�̐����t���O
			featureLevels ,		// �@�\���x��
			numFeatureLevels ,	// �@�\���x����
			D3D11_SDK_VERSION ,	// Dx11 �o�[�W����
			&swapChainDesc ,	// �X���b�v�`�F�C���̐ݒ�
			&m_swapChain ,		// IDXGIDwapChain�C���^�t�F�[�X	
			&m_device ,			// ID3D11Device�C���^�t�F�[�X
			&m_featureLevel ,	// �T�|�[�g����Ă���@�\���x��
			&pDevContext );		// �f�o�C�X�R���e�L�X�g
		if( SUCCEEDED( hr ) )break;
	}
	if( FAILED( hr ) ){ return TS_FALSE; }

	//! Create Context 
	m_deviceContext = TsNew( TsDeviceContext( this , pDevContext ) );
	TsRenderTarget* rtv = TsRenderTarget::CrateScreenRTV( *this );
	m_deviceContext->SetScreenRTV( rtv );

	TsDepthStencil* depthStencil = 
		TsDepthStencil::CreateDSV( "MainDepthStencil",*this,width,height);
	m_deviceContext->SetDepthStencil( depthStencil );

	m_deviceContext->ResetDrawCallCount();
	m_deviceContext->SetRT( 0 , rtv );
	m_deviceContext->ApplyRenderTargets();

	TsViewport viewport;
	viewport.Create( width , height );
	m_deviceContext->SetViewport( &viewport );

	CreateAndSetRaster();

	LoadDefaultShader();
	
	return TS_TRUE;
}

//=============================================
// ! GetRefreshRato
// PC ���j�^�̃��t���b�V�����[�g���擾
// @pram _out refreshRato x = ���q�@y =����
//		 width & height ��ʃT�C�Y
// @return success is true
TsBool TsDevice::GetRefreshRato( __out TsUint2& refreshRato ,
								 const TsUint width , const TsUint height )
{
	refreshRato.x = 1;
	refreshRato.y = 60;
	//�ϐ�
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

TsBool TsDevice::CreateAndSetRaster()
{
	HRESULT hr;
	D3D11_RASTERIZER_DESC rasterDesc;
	ID3D11RasterizerState* rasterState;

	// setup the raster description which will determine how and what polygons will be drawn
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = TS_FALSE;
	rasterDesc.MultisampleEnable = TS_FALSE;
	rasterDesc.ScissorEnable = TS_FALSE;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// create the rasterrizer state from the description we just filled out 
	hr = m_device->CreateRasterizerState( &rasterDesc , &rasterState );
	if( FAILED( hr ) )
	{
		return TS_FALSE;
	}


	return TS_TRUE;
}

ID3D11Buffer* TsDevice::CreateBuffer( __in void * pData ,
									  __in size_t size ,
									  __in UINT cpuAct ,
									  __in UINT type )const
{
	HRESULT hr = E_FAIL;

	// �o�b�t�@�[ ���\�[�X�B
	D3D11_BUFFER_DESC BufferDesc;

	// �T�u���\�[�X
	D3D11_SUBRESOURCE_DATA resource ;

	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;

	if( cpuAct )
		Usage = D3D11_USAGE_DYNAMIC;
	else
		Usage = D3D11_USAGE_DEFAULT;

	if( pData )
	{
		resource.pSysMem = pData;
		resource.SysMemPitch = 0;
		resource.SysMemSlicePitch = 0;
	}

	// �o�b�t�@�̐ݒ�
	::ZeroMemory( &BufferDesc , sizeof( BufferDesc ) );
	BufferDesc.ByteWidth = size;                  // �o�b�t�@�T�C�Y
	BufferDesc.Usage = Usage;                     // ���\�[�X�g�p�@����肷��
	BufferDesc.BindFlags = type;                  // �o�b�t�@�̎��
	BufferDesc.CPUAccessFlags = cpuAct;           // CPU �A�N�Z�X
	BufferDesc.MiscFlags = 0;                     // ���̑��̃t���O���ݒ肵�Ȃ�


	ID3D11Buffer* pBuffer;

	// �o�b�t�@���쐬����
	hr = GetDevD3D()->CreateBuffer( &BufferDesc , &resource , &pBuffer );

	if( FAILED( hr ) )
	{
		TsDebugLog( "Create Buffer Error\n" );
		return nullptr;
	}

	return pBuffer;

}

TsCBuffer* TsDevice::CreateCBuffer(__in void * pData ,
								   __in size_t size )const
{
	ID3D11Buffer* pBuffer = CreateBuffer( pData , size , 
										  D3D11_CPU_ACCESS_WRITE , 
										  D3D11_BIND_CONSTANT_BUFFER );
	if (pBuffer == nullptr)
	{
		TsDebugLog("Create Buffer Error\n");
		return nullptr;
	}
	TsCBuffer * pCBuffer = TsNew( TsCBuffer() );
	pCBuffer->SetD3DBufferAndSize(  pBuffer ,size);

	return pCBuffer;
}

TsIndexBuffer* TsDevice::CreateIndexBuffer( __in void * pData ,
											__in size_t size )const
{
	ID3D11Buffer* pBuffer = CreateBuffer( pData , size ,
										  0 ,
										  D3D11_BIND_INDEX_BUFFER );
	TsIndexBuffer * pIB = TsNew( TsIndexBuffer() );
	pIB->SetD3DBufferAndSize( pBuffer , size );

	return pIB;
}

TsVertexBuffer* TsDevice::CreateVertexBuffer( __in void * pData ,
											  __in size_t size , 
											  size_t stride,
											  TsUint offset )const
{
	ID3D11Buffer* pBuffer = CreateBuffer( pData , size ,
										  0 ,
										  D3D11_BIND_VERTEX_BUFFER );
	TsVertexBuffer * pVB = TsNew( TsVertexBuffer() );
	pVB->SetD3DBufferAndSize( pBuffer , size );
	pVB->SetStride( stride );
	pVB->SetOffset( offset );

	return pVB;
}

TsBool TsDevice::LoadDefaultShader()
{
	if( m_device == nullptr )
		return TS_FALSE;

	TsVertexShader * pSSDefaultVS = TsNew( TsVertexShader );

	pSSDefaultVS->LoadFromCSO( m_device, (TSUT::Resource::GetCSODirectory() + "TsSSDefaultVS.cso").c_str() );

	return TS_TRUE;
}

TsBool TsDevice::Flip()
{
	if( m_device && m_swapChain )
		m_swapChain->Present( 0 , 0 );
	else
		return TS_FALSE;
	return TS_TRUE;
}