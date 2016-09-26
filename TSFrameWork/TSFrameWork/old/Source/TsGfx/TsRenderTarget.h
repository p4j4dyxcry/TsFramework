//!*******************************************************
//!	TsRenderTaeget.h
//!
//!	
//!
//!	© 2016 Yuki Tsuneyama
#pragma once

class TsRenderTarget : public TsTexture2D , ITsHasStaticObject<TsRenderTarget>
{
public:
	virtual ~TsRenderTarget();

	ID3D11RenderTargetView* GetRTV(){ return m_rtv; }
	TsInt2 GetRTVSize()const;

	//failed is nullptr
	static TsRenderTarget* CrateScreenRTV( const TsDevice& dev );
protected:
	void operator = ( TsRenderTarget* ) = delete;
	TsRenderTarget( const TsRenderTarget& ) = delete;
	TsRenderTarget();

	TsBool Create( const TsDevice& dev ,
				   const TsInt width ,
				   const TsInt height ,
				   DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM );
	void UnInit();

private:
	ID3D11RenderTargetView*	  m_rtv;
	TsInt2					  m_rtvSize;
};