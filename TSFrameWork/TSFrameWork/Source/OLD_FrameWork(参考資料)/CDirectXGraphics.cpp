#include	"CDirectXGraphics.h"
//#include	"DebugConsole.h"
/*-------------------------------------------------------------------------------
	DirectX Grpaphics �̏���������

		P1 : �E�C���h�E�n���h���l
		P2 : �t���X�N���[���t���O�i�O�F�E�C���h�E���[�h�@�P�F�t���X�N���[���j
		P3 : �E�C���h�E�T�C�Y�i���j
		P4 : �E�C���h�E�T�C�Y�i�����j

		�߂�l
			false : ���s�@
			true  : ����
---------------------------------------------------------------------------------*/
bool CDirectXGraphics::Init(HWND hWnd,int fullscreenflag,unsigned int Width,unsigned int Height,bool fullscreen){
	HRESULT	hr=S_OK;
	IDXGIFactory* factory;				// factory
	IDXGIAdapter* adapter;				// videocard
	IDXGIOutput* adapterOutput;			// monitor
	unsigned int numModes;
	unsigned int numerator=60;			// ���q
	unsigned int denominator=1;			// ����
	DXGI_MODE_DESC* displayModeList;

	m_Width = Width;
	m_Height = Height;

	// �h���C�o�̎��
	D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,				
        D3D_DRIVER_TYPE_WARP,					// Windows Advanced Rasterizer
        D3D_DRIVER_TYPE_REFERENCE,
    };
    unsigned int numDriverTypes = ARRAYSIZE( driverTypes );

    unsigned int createDeviceFlags = 0;
 //   createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;				// ���̃t���O���g�p����Ə����������s����

	// �@�\���x��
    D3D_FEATURE_LEVEL featureLevels[] =
    {
		D3D_FEATURE_LEVEL_11_1,					// DirectX11.1�Ή�GPU���x��
		D3D_FEATURE_LEVEL_11_0,					// DirectX11�Ή�GPU���x��
        D3D_FEATURE_LEVEL_10_1,					// DirectX10.1�Ή�GPU���x��
        D3D_FEATURE_LEVEL_10_0,					// DirectX10�Ή�GPU���x��
        D3D_FEATURE_LEVEL_9_3,					// DirectX9.3�Ή�GPU���x��
        D3D_FEATURE_LEVEL_9_2,					// DirectX9.2�Ή�GPU���x��
        D3D_FEATURE_LEVEL_9_1					// Direct9.1�Ή�GPU���x��
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	// Create a DirectX graphics interface factory
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(hr)){
		return false;
	}

	// use the factory to create an adpter for the primary graphics interface(video card)
	hr = factory->EnumAdapters(0, &adapter);
	if (FAILED(hr)){
		return false;
	}

	// enumerrate primary adapter output(monitor)
	hr = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(hr)){
		return false;
	}

	// get the number of modes that fit the DXGI_FORMAT_R8G8B8_UNORM display format forthe adapter output(monitor)
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(hr)){
		return false;
	}
	
	// create alist to hold all possible display modes for this monitor/video card combination
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList){
		return false;
	}

	// now fill the display mode list structures
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(hr)){
		return false;
	}

	// now go through all the display modes and find the one that matches the screen width and height
	// when a match is found store the numerator and denominator of the refresh rate for that monitor
	for (unsigned int i = 0; i < numModes; i++){
		if (displayModeList[i].Width == Width){
			if (displayModeList[i].Height == Height){
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	delete[] displayModeList;
	displayModeList = 0;

	adapterOutput->Release();
	adapterOutput = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;

	// �X���b�v�`�F�C���̐ݒ�
	// �X���b�v�`�F�C���Ƃ́A�E�C���h�E�ւ̕\���_�u���o�b�t�@���Ǘ�����
	// �N���X�@�}���`�T���v�����O�A���t���b�V�����[�g���ݒ�ł���
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );					// �[���N���A
    sd.BufferCount = 1;									// �o�b�N�o�b�t�@�̐�
    sd.BufferDesc.Width = Width;						// �o�b�N�o�b�t�@�̕�
    sd.BufferDesc.Height = Height;						// �o�b�N�o�b�t�@�̍���
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �o�b�N�o�b�t�@�t�H�[�}�b�g(R,G,B �͈͂O�D�O����P�D�O)
    sd.BufferDesc.RefreshRate.Numerator = numerator;			// ���t���b�V�����[�g�i����j
	sd.BufferDesc.RefreshRate.Denominator = denominator;			// ���t���b�V�����[�g�i���q�j
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;			
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �o�b�N�o�b�t�@�̎g�p���@
    sd.OutputWindow = hWnd;			// �֘A�t����E�C���h�E
    sd.SampleDesc.Count = 1;		// �}���`�T���v���̐�
    sd.SampleDesc.Quality = 0;		// �}���`�T���v���̃N�I���e�B
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	

	if (fullscreen){
		sd.Windowed = false;				// �E�C���h�E���[�h
	}
	else{
		sd.Windowed = TRUE;				// �E�C���h�E���[�h
	}
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		// ���[�h�̎����؂�ւ�

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        m_DriverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain( NULL,				// �f�B�X�v���C�f�o�C�X�̃A�_�v�^�i�m�t�k�k�̏ꍇ�ŏ��Ɍ��������A�_�v�^�j
											m_DriverType,		// �f�o�C�X�h���C�o�̃^�C�v
											NULL,				// �\�t�g�E�F�A���X�^���C�U���g�p����ꍇ�Ɏw�肷��
											createDeviceFlags,	// �f�o�C�X�t���O
											featureLevels,		// �@�\���x��
											numFeatureLevels,	// �@�\���x����
                                            D3D11_SDK_VERSION,	// 
											&sd,				// �X���b�v�`�F�C���̐ݒ�
											&m_lpSwapChain,		// IDXGIDwapChain�C���^�t�F�[�X	
											&m_lpDevice,		// ID3D11Device�C���^�t�F�[�X
											&m_FeatureLevel,	// �T�|�[�g����Ă���@�\���x��
											&m_lpImmediateContext );	// �f�o�C�X�R���e�L�X�g
        if( SUCCEEDED( hr ) )
            break;
    }
	if( FAILED( hr ) ){
        return false;
	}

    // �����_�����O�^�[�Q�b�g���쐬
	// �o�b�N�o�b�t�@�̃|�C���^���擾
	ID3D11Texture2D* pBackBuffer = NULL;
    hr = m_lpSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if( FAILED( hr ) )
        return false;

	// �o�b�N�o�b�t�@�ւ̃|�C���^���w�肵�ă����_�[�^�[�Q�b�g�r���[���쐬
	hr = m_lpDevice->CreateRenderTargetView( pBackBuffer, NULL, &m_lpRenderTargetView );
	pBackBuffer->Release();
	pBackBuffer = 0;
	if (FAILED(hr))
		return false;
	
	// Z�o�b�t�@
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	
	depthBufferDesc.Width = Width;
	depthBufferDesc.Height = Height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	hr = m_lpDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(hr)){
		return false;
	}

	// �X�e���V���X�e�[�g�쐬

	// �X�e���V���ݒ�\���̏�����
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// set up the description of the stencl state
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// stencil operation if pixel is front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// stencil operation if pixel is�@back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// create the depth stencil state
	hr = m_lpDevice->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(hr)){
		return false;
	}

	// �f�o�C�X�R���e�L�X�g�փZ�b�g
	m_lpImmediateContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// depthstencilview ������
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	// set up the depth stencil view description
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// create the depth stencil view
	hr = m_lpDevice->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(hr)){
		return false;
	}

	// bind the render target view and depth stencil buffer to the output render pipeline
    m_lpImmediateContext->OMSetRenderTargets( 1, &m_lpRenderTargetView, m_depthStencilView );

	// setup the raster description which will determine how and what polygons will be drawn
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// create the rasterrizer state from the description we just filled out 
	hr = m_lpDevice->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(hr)){
		return false;
	}

	//  set the rasterizer state
	m_lpImmediateContext->RSSetState(m_rasterState);


    // �r���[�|�[�g��ݒ�
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)Width;
    vp.Height = (FLOAT)Height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_lpImmediateContext->RSSetViewports( 1, &vp );

	m_Height = Height;
	m_Width  = Width;

	return(true);
}
/*-------------------------------------------------------------------------------
	DirectX Grpaphics �̏I������
---------------------------------------------------------------------------------*/
void	CDirectXGraphics::Exit()
{
	if (m_lpImmediateContext){
		m_lpImmediateContext->ClearState();
	}
	if (m_lpSwapChain) {
		m_lpSwapChain->SetFullscreenState(false, NULL);
	}
	
	if (m_rasterState) {
		m_rasterState->Release();
		m_rasterState= 0;
	}

	if (m_depthStencilBuffer) {
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if (m_depthStencilState) {
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}
	if (m_depthStencilView){
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if (m_lpRenderTargetView){
		m_lpRenderTargetView->Release();
		m_lpRenderTargetView = 0;
	}

	if (m_lpImmediateContext){
		m_lpImmediateContext->Release();
		m_lpImmediateContext = 0;
	}

	if (m_lpDevice){
		m_lpDevice->Release();
		m_lpDevice = 0;
	}

	if (m_lpSwapChain) {
		m_lpSwapChain->Release();
		m_lpSwapChain = 0;
	}
	return;
}

void CDirectXGraphics::CreateBuffer( 
						__out ID3D11Buffer ** pBuffer ,
						__in void * pData ,
						__in size_t size ,
						__in UINT cpuAct ,
						__in UINT type  )const
{
	HRESULT hr = E_FAIL;

	// �o�b�t�@�[ ���\�[�X�B
	// D3D11_BUFFER_DESC
	D3D11_BUFFER_DESC BufferDesc;

	// �T�u���\�[�X
	// D3D11_SUBRESOURCE_DATA
	D3D11_SUBRESOURCE_DATA* resource = NULL;

	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
	UINT CPUAccessFlags = cpuAct;

	switch( CPUAccessFlags )
	{
		// CPU�A�N�Z�X�������Ȃ�
		case 0:
			Usage = D3D11_USAGE_DEFAULT;
			break;
			// CPU�A�N�Z�X��������
		default:
			Usage = D3D11_USAGE_DYNAMIC;
			break;
	}

	if( pData )
	{
		resource = new D3D11_SUBRESOURCE_DATA();
		resource->pSysMem = pData;
		resource->SysMemPitch = 0;
		resource->SysMemSlicePitch = 0;
	}

	// �o�b�t�@�̐ݒ�
	::ZeroMemory( &BufferDesc , sizeof( BufferDesc ) );
	BufferDesc.ByteWidth = size;                      // �o�b�t�@�T�C�Y
	BufferDesc.Usage = Usage;                     // ���\�[�X�g�p�@����肷��
	BufferDesc.BindFlags = type;                  // �o�b�t�@�̎��
	BufferDesc.CPUAccessFlags = CPUAccessFlags;            // CPU �A�N�Z�X
	BufferDesc.MiscFlags = 0;                         // ���̑��̃t���O���ݒ肵�Ȃ�

	// �o�b�t�@���쐬����
	hr = GetDXDevice()->CreateBuffer( &BufferDesc , resource , pBuffer );

	delete resource;

}