//!*******************************************************
//!	TsDepthBuffer.h
//!
//!	
//!
//!	© 2016 Yuki Tsuneyama

class TsDepthStencil :public TsTexture2D ,
					  public ITsStaticNameObjectList<TsDepthStencil>
{
public:
	//! コンストラクタ
	TsDepthStencil() :m_depthStencil( nullptr ){};

	//! デストラクタ
	virtual ~TsDepthStencil(){ TsSafeRelease( m_depthStencil ); }

	//Depth Stencil Viewの作成
	static TsDepthStencil* CreateDSV( TsString name,
									  const TsDevice& dev ,
									  TsInt width ,
									  TsInt height ,
									  DXGI_FORMAT format = DXGI_FORMAT_D24_UNORM_S8_UINT );

	// Dx11レイヤーの Deptsh Stencil Stateの取得
	ID3D11DepthStencilState* GetDSS()const{ return m_depthStencil; }

	// Dx11レイヤーの Deptsh Stencil Viewの取得
	ID3D11DepthStencilView * GetDSV()const{ return m_depthStencilView; }
private:

	//作成
	TsBool Create( const TsDevice& dev ,
				   TsInt width ,
				   TsInt height ,
				   DXGI_FORMAT format);

	ID3D11DepthStencilState* m_depthStencil;
	ID3D11DepthStencilView * m_depthStencilView;
};