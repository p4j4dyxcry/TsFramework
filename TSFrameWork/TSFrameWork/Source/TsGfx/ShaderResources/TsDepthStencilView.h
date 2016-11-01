//!*******************************************************
//!	TsDepthBuffer.h
//!
//!	Depth & Stencil
//!
//! © 2016 Yuki Tsuneyama

class TsDepthStencilView :  public TsTexture2D ,
                            public ITsStaticNameObjectList<TsDepthStencilView>
{
public:
    //! コンストラクタ
    TsDepthStencilView() :m_depthStencilView( nullptr ){};

    //! デストラクタ
    virtual ~TsDepthStencilView(){  }

    //Depth Stencil Viewの作成
    static TsDepthStencilView* CreateDSV( TsString name,
                                      const TsDevice& dev ,
                                      TsInt width ,
                                      TsInt height ,
                                      DXGI_FORMAT format = DXGI_FORMAT_D24_UNORM_S8_UINT );

    // Dx11レイヤーの Deptsh Stencil Viewの取得
    ID3D11DepthStencilView * GetDSV()const{ return m_depthStencilView; }
private:

    //作成
    TsBool Create( const TsDevice& dev ,
                   TsInt width ,
                   TsInt height ,
                   DXGI_FORMAT format);

    ID3D11DepthStencilView * m_depthStencilView;
};