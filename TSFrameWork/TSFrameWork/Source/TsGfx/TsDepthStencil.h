//!*******************************************************
//!	TsDepthBuffer.h
//!
//!	
//!
//!	© 2016 Yuki Tsuneyama

class TsDepthStencil :public TsTexture2D , ITsStaticNameObjectList<TsDepthStencil>
{
public:
	TsDepthStencil() :m_depthStencil( nullptr ){};
	virtual ~TsDepthStencil(){ TsSafeRelease( m_depthStencil ); }

	static TsDepthStencil* CreateDSV( TsString name,
									  const ITsDevice& dev ,
									  TsInt width ,
									  TsInt height ,
									  DXGI_FORMAT format = DXGI_FORMAT_D24_UNORM_S8_UINT );

	ID3D11DepthStencilState* GetDSS()const{ return m_depthStencil; }
	ID3D11DepthStencilView * GetDSV()const{ return m_depthStencilView; }
private:
	TsBool Create( const ITsDevice& dev ,
				   TsInt width ,
				   TsInt height ,
				   DXGI_FORMAT format);

	ID3D11DepthStencilState* m_depthStencil;
	ID3D11DepthStencilView * m_depthStencilView;
};