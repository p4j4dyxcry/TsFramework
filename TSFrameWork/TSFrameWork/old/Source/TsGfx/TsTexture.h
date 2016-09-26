//!*******************************************************
//!	TsTexture.h
//!
//!	Dx11 Texture Lapping.
//!
//!	© 2016 Yuki Tsuneyama
#pragma once

class TsTexture 
{
public:
	TsTexture() :m_srv( nullptr ){};
	virtual ~TsTexture()
	{
		TsSafeRelease( m_srv );
	}
	ID3D11ShaderResourceView * GetSRV()const {return m_srv;}
	TsString GetName()const{ return m_name; }
	void Setname( TsString str ){ 
		m_name = str; 
		m_hashCode = TSUT::StringToHash( str );
	}
	TS_HASH GetHashCode()const{ return m_hashCode; }
protected:
	ID3D11ShaderResourceView * m_srv;
	TsString m_name;
private:
	TS_HASH	 m_hashCode;
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
