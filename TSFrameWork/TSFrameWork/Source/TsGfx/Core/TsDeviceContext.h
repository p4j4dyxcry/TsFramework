//!*******************************************************
//! TsDeviceContext.h
//!
//! lapping dx11 device context.
//!
//! © 2016 Yuki Tsuneyama

//----------------------------------------------------------
// Define
//----------------------------------------------------------
class TsRenderTarget;
class TsDepthStencilView;
class TsBlendState;
class TsTexture;
class TsShaderEffect;
class TsBuffer;
class TsCBuffer;
class TsViewport;
class TsCamera;

class TsDeviceContext
{
public:
    //----------------------------------------------------------
    // Define
    //----------------------------------------------------------
    static const TsInt	 MAX_RTs = D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;

    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------

    TsDeviceContext( TsDevice* device , ID3D11DeviceContext* pDevContext );
    virtual ~TsDeviceContext();

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

    //=============================================
    // ! SetSamplerState
    // デバイスにサンプラーステートを設定
    // @return true on success
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

    //=============================================
    // ! SetDepthStencilState
    // パイプラインに設定する深度/ステンシルの設定
    // @return True on Success
    TsBool SetDepthStencilState( TsDepthStencilState * pDepthStencil );
    
    //=============================================
    // ! SetRasterizerState
    // パイプラインに設定するラスタライズステートの設定
    // @return True on Success
    TsBool SetRasterizerState( TsRasterizerState * pRasterState );

    //=============================================
    // ! SetBlendState
    // パイプラインに設定するブレンドステートの設定
    // @return True on Success
    TsBool SetBlendState( TsBlendState * pBlendState );

    //! DepthStencilStateを取得する
    const TsDepthStencilState* GetDepthStencilState()const{ return m_pDepthStencilState; };
    TsDepthStencilState* GetDepthStencilState(){ return m_pDepthStencilState; }

    //! BlendStateを取得する
    const TsRasterizerState* GetRasterState()const{ return m_pRasterizerState; };
    TsRasterizerState* GetRasterState(){ return m_pRasterizerState; }

    //! BlendStateを取得する
    const TsBlendState* GetBlendState()const{ return m_pBlendState; };
    TsBlendState* GetBlendState(){ return m_pBlendState; }

    //=============================================
    // ! SetCBuffer
    // CBuffer をパイプラインにセットする
    // @return True on Success
    TsBool SetCBuffer(const TsCBuffer * cbuffer);

    //=============================================
    // ! ChangeCBuffer
    // CBuffer の更新をロックし更新する
    // @return True on Success
    TsBool ChangeCBuffer(TsCBuffer * cbuffer, void * pData, size_t sz);

    void* Map(TsBuffer* pBuffer,D3D11_MAP mapType);

    TsBool UnMap(TsBuffer* pBuffer);

    //=============================================
    // ! SetAndChangeCBuffer
    // CBuffer の更新　かつ　パイプラインにセットする
    // @return True on Success
    TsBool SetAndChangeCBuffer( TsCBuffer* cbuffer , 
                               void * pData , 
                               size_t size );

    //=============================================
    // ! Draw
    // ジオメトリを頂点をGPUに転送する
    // @return True on Success
    TsBool Draw( TsInt vtxNum , TsInt startSlot = 0);

    //=============================================
    // ! DrawIndex
    // ジオメトリを頂点をIndexBufferを元にGPUに転送する
    // @return True on Success
    TsBool DrawIndex( TsInt index , TsInt startSlot = 0 , TsInt indexLocation = 0 );

    //=============================================
    // ! Draw Instance
    // ジオメトリを頂点をGPUに転送する
    // @return True on Success
    TsBool DrawInstance( TsInt vtxNum , 
                         TsInt instanceNum , 
                         TsInt instanceLocation = 0 ,
                         TsInt startSlot = 0 );

    //=============================================
    // ! DrawIndex Instance
    // ジオメトリを頂点をGPUに転送する
    // @return True on Success
    TsBool DrawIndexInstance( TsInt index , 
                              TsInt instanceNum , 
                              TsInt instanceLocation = 0, 
                              TsInt startSlot = 0 , 
                              TsInt indexLocation = 0 );


    //! todo 未使用
    TsBool ResetDrawCallCount();

    //=============================================
    // ! SetViewport
    //  Viewport をパイプラインに設定する
    // @return True on Success
    TsBool SetViewport( TsViewport* viewport );

    //=============================================
    // ! ApplyInputLayout
    //  インプットレイアウトの確定
    // @return True on Success
    TsBool ApplyInputLayout();

    //=============================================
    // ! ApplyInputLayout
    //  深度＆ステンシルの確定
    // @return True on Success
    TsBool ApplyDepthStencil();

    //=============================================
    // ! ApplyInputLayout
    //  ラスタライズステートの確定
    // @return True on Success
    TsBool ApplyRasterizer();

    //=============================================
    // ! ApplyInputLayout
    //  ブレンドステートの確定
    // @return True on Success
    TsBool ApplyBlendState();

    //=============================================
    // ! SetVertexBuffer
    //  VertexBufferをパイプラインに設定
    // @return True on Success
    TsBool SetVertexBuffer( TsVertexBuffer* );

    //=============================================
    // ! SetIndexBuffer
    //  IndexBufferをデバイスに設定
    // @return True on Success
    TsBool SetIndexBuffer( TsIndexBuffer* );

    //! todo 未作成
    TsBool ClearVertexBuffer( );
    //! todo 未作成
    TsBool ClearIndexBuffer( );

    //=============================================
    // ! ClearCBuffer
    //  GPUメモリのCBufferResouceをクリアする
    // @return True on Success
    TsBool ClearCBuffer( TsInt index = -1 , TS_SHADER_TYPE type = TS_SHADER_TYPE::ALL_SHADER );

    //=============================================
    // ! GetMainCamera
    //  パイプラインに設定されているカメラを取得する。
    //  パイプラインにカメラが設定されていない場合は
    //  デフォルトカメラを作成する。
    // @return True on Success
    TsCamera* GetMainCamera()const;

    //=============================================
    // ! SetMainCamera
    //  パイプラインにカメラを設定する。
    //  nullを指定した場合デフォルトカメラが使用される。
    // @return True on Success
    TsBool SetMainCamera(TsCamera*);

    //=============================================
    // ! SetTopology
    // 頂点の並び構造を設定する
    // @return True on Success
    TsBool SetTopology( D3D_PRIMITIVE_TOPOLOGY topology );
private:    

    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------    
    TsDevice*                              m_pDevice;
    ID3D11DeviceContext*                   m_pDeviceContext;
    TsShaderEffect*                        m_bindShaderEffect;
    TsBool                                 m_activeShaders[TS_SHADER_INDEX::SHADER_MAX_IDX];
    TsRenderTarget *                       m_renderTarget[MAX_RTs];
    TsRenderTarget *                       m_mainRenderTarget;
    TsDepthStencilView *                   m_mainDepthStencil;
    TsDepthStencilView *                   m_depthStencil;

    TsBlendState*                          m_pBlendState;
    TsRasterizerState *                    m_pRasterizerState;
    TsDepthStencilState*                   m_pDepthStencilState;

    TsCamera*                              m_mainCamera;
    TsInt                                  m_drawCallCount;
};