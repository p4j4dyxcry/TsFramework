//!*******************************************************
//! TsDeviceContext.h
//!
//! lapping dx11 device context.
//!
//! © 2016 Yuki Tsuneyama

class TsRenderTarget;
class TsDepthStencilView;
class TsTexture;
class TsShaderEffect;
class TsCBuffer;
class TsViewport;
class TsCamera;

class TsDeviceContext
{
public:
    static const TsInt	 MAX_RTs = D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;

    TsDeviceContext( TsDevice* device , ID3D11DeviceContext* pDevContext );
    virtual ~TsDeviceContext();
    //todo functions.

    //=============================================
    // ! Clear
    // バインドされているRTV及びDSVのクリア
    // @pram color	RTVをクリアする色
    void Clear( TsFloat4& color = TsFloat4( 0 , 1 , .5f , .5f ) );

    //=============================================
    // ! SetRT
    // RenderTargetView(RTV)を登録を依頼
    // @pram idx 0 - 7 
    // @pram rt  RTV
    // @return True on Success 
    TsBool SetRT( TsInt idx , TsRenderTarget* rt );

    //=============================================
    // ! SetScreenRTV
    // ディスプレイに出力するRenderTargetView(RTV)を登録
    // ※このメソッドは明示的に呼ぶ必要はない
    // @pram rtv SwapChainから取得したバックバッファで生成
    //           されたRTV
    // @return True on Success 
    TsBool SetScreenRTV( TsRenderTarget* rtv );

    //=============================================
    // ! SetDepth
    // DepthStencilViewを登録
    // @pram dsv 
    // @return True on Success 
    TsBool SetDepthStencilView(TsDepthStencilView* dsv);

    //=============================================
    // ! SetDepth
    // DepthStencilViewを登録
    // @pram dsv 
    // @return True on Success 
    TsBool SetMainDepthStencil( TsDepthStencilView* dsv );

    //=============================================
    // ! SetTexture
    // テクスチャを登録
    // @pram slotIndex 0-7
    // @pram texture テクスチャ　OR　RTV
    // @pram type どのシェーダにバインドするか(通常はPS)
    // @return True on Success 
    TsBool SetTexture( TsInt slotIndex , 
                       const TsTexture* texture , 
                       TS_SHADER_TYPE type );

    TsBool SetSamplerState( TsSamplerState* pSampler , 
                            TsInt registerIndex ,
                            TS_SHADER_TYPE type = TS_SHADER_TYPE::PIXEL_SHADER);

    //=============================================
    // ! SetShader
    // シェーダの集合を登録
    // @pram shaderEffect
    // @return True on Success 
    TsBool SetShader( TsShaderEffect* shaderEffect );

    //=============================================
    // ! ApplyRenderTargets
    // RenderTargetの変更を登録
    // ※このメソッドはパスの開始時に明示的に呼び出すこと。
    // @return True on Success 
    TsBool ApplyRenderTargets();

    //=============================================
    // ! GetScreenRTV
    // ディスプレイにバインドされているRTVを取得
    // @return ディスプレイにバインドされているRTV
    TsRenderTarget* GetMainRTV()const{ return m_mainRenderTarget; }

    //=============================================
    // ! GetMainDSV
    // ディスプレイにバインドされているRTVを取得
    // @return ディスプレイにバインドされているRTV
    TsDepthStencilView* GetMainDSV()const{ return m_mainDepthStencil; }

    TsBool SetDepthStencilState( TsDepthStencilState * pDepthStencil );
    TsBool SetRasterizerState( TsRasterizerState * pRasterState );

    TsBool SetAndChangeCBuffer( TsCBuffer* cbuffer , 
                               void * pData , 
                               size_t size );

    TsBool SetCBuffer(const TsCBuffer * cbuffer );
    TsBool ChangeCBuffer( TsCBuffer * cbuffer , void * pData , size_t sz );

    TsBool Draw( TsInt vtxNum , TsInt startSlot = 0);
    TsBool DrawIndex( TsInt index , TsInt startSlot = 0 , TsInt indexLocation = 0 );

    TsBool ResetDrawCallCount();


    TsBool SetViewport( TsViewport* viewport );

    TsBool ApplyInputLayout();

    TsBool ApplyDepthStencil();
    TsBool ApplyRasterizer();

    TsBool SetVertexBuffer( TsVertexBuffer* );
    TsBool SetIndexBuffer( TsIndexBuffer* );

    TsCamera* GetMainCamera()const;
    TsBool SetMainCamera(TsCamera*);
    TsBool SetTopology( D3D_PRIMITIVE_TOPOLOGY topology );
private:
    
    TsDevice*                                     m_pDevice;
    ID3D11DeviceContext*               m_pDeviceContext;
    TsShaderEffect*                           m_bindShaderEffect;
    TsBool                                          m_activeShaders[TS_SHADER_INDEX::MAX_IDX];
    TsRenderTarget *                        m_renderTarget[MAX_RTs];
    TsRenderTarget *                        m_mainRenderTarget;
    TsDepthStencilView *                 m_mainDepthStencil;
    TsDepthStencilView *                 m_depthStencil;

    TsRasterizerState *                    m_pRasterizerState;
    TsDepthStencilState*                m_pDepthStencilState;

    TsCamera*                                 m_mainCamera;
    TsInt                                           m_drawCallCount;
};