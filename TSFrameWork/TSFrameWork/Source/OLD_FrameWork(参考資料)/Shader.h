//!*******************************************************
//!	Shader.h
//!
//!	シェーダクラス
//!
//!	著作権	2016 Yuki Tsuneyama
//!	作成日時	16.01.15

#pragma once

#include "Common.h"
#include <d3d11.h>

//!*******************************************************
//!	[クラス] シェーダリソースビュー
//!
/*
class CShaderResouceView
{
public:
	CShaderResouceView() :m_pView( nullptr ){};
	virtual ~CShaderResouceView()
	{
		SafeRelease( m_pView );
	}

	void Initialize( ID3D11Device* pDev ,
					 ID3D11Buffer* pBuffer ,
					 D3D11_SHADER_RESOURCE_VIEW_DESC* desc );

	inline const ID3D11ShaderResourceView* Get()const
	{
		return m_pView;
	}

protected:
	ID3D11ShaderResourceView*		m_pView;
};*/

//!*******************************************************
//!	[クラス] シェーダクラス
//!

template<typename Shader>
class CShader
{
public :
	CShader() :m_shader( nullptr ) , m_inputLayout( nullptr ),m_resources(nullptr){}
	

	inline void CompileFromFile( __in ID3D11Device* pDev ,
						  __in LPCWSTR filename ,
						  __in CHAR* entryPoint ,
						  __in CHAR* pProfile );

	inline void CompileFromCSO(  __in ID3D11Device * pDev,
						  __in CHAR* filename );

	inline void CompileFromMemory( ID3D11Device* pDev,
								   void* memory ,
								   size_t size);

	inline void CreateInputLayout( ID3D11Device* pDev );

	ID3D11InputLayout* GetInputLayout()const{ return m_inputLayout; }
	Shader* GetShader()const { return m_shader; }

	virtual ~CShader()
	{
		SafeRelease( m_shader );
		SafeRelease( m_inputLayout );
		SafeDelete( m_resources );
	};
protected:
	Shader*				m_shader;
	ID3D11InputLayout*	m_inputLayout;
	BYTE*				m_resources;
	size_t				m_resourcesSize;
};

typedef CShader<ID3D11VertexShader>		VertexShader;
typedef CShader<ID3D11PixelShader>		PixelShader;
typedef CShader<ID3D11GeometryShader>	GeometryShader;
typedef CShader<ID3D11HullShader>		HullShader;
typedef CShader<ID3D11DomainShader>		DomainShader;
typedef CShader<ID3D11ComputeShader>	ComputeShader;

#include "Shader.inl"