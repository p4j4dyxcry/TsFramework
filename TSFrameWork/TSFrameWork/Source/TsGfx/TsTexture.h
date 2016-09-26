//!*******************************************************
//!	TsTexture.h
//!
//!	Dx11 Texture Lapping.
//!
//!	© 2016 Yuki Tsuneyama
#pragma once

class TsTexture : public TsShaderResource , 
				  public TsNameObject 
{
public:
	TsTexture() :m_srv( nullptr ){};
	virtual ~TsTexture()
	{
		TsSafeRelease( m_srv );
	}
	ID3D11ShaderResourceView * GetSRV()const {return m_srv;}
	TsBool SetSRV( ID3D11ShaderResourceView*srv )
	{
		if( m_srv )
			return TS_FALSE;
		m_srv = srv; 
		return TS_TRUE;
	}
protected:
	ID3D11ShaderResourceView * m_srv;
	TsInt m_registerIndex = 0;
};

class TsTexture2D : public TsTexture
{
public:
	TsTexture2D() :m_tex2d( nullptr ){};
	virtual ~TsTexture2D()
	{
		TsSafeRelease( m_tex2d );
	}

protected:
	ID3D11Texture2D * m_tex2d;
};

class TsCubeMap : TsTexture
{
	//todo
protected:
	ID3D11Texture2D * m_tex2d[6];
};

class TsTexture3D : TsTexture
{
	//todo
protected:
	ID3D11Texture3D * m_tex3d;
};

class TsTexture1D : TsTexture
{
	//todo
protected:
	ID3D11Texture1D * m_tex1d;
};
