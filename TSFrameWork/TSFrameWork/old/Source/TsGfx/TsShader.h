//!*******************************************************
//!	TsShader.h
//!
//!	Shader Template
//!
//!	© 2016 Yuki Tsuneyama
#pragma once

template<typename Shader>
class TsShader : public TsBinary
{
public:
	TsShader() :m_shader( nullptr ) , m_inputLayout( nullptr )
	{
		this->TsBinary::TsBinary();
	}

	inline TsBool CompileFromFile( __in ID3D11Device* pDev ,
								   __in const TsLChar* filename ,
								   __in const TsChar* entryPoint ,
								   __in const TsChar* pProfile );

	inline TsBool LoadFromCSO( __in ID3D11Device * pDev ,
							   __in const TsChar* filename );

	inline TsBool CompileFromMemory( ID3D11Device* pDev ,
								   void* memory ,
								   size_t size );

	inline TsBool CreateInputLayout( ID3D11Device* pDev );

	ID3D11InputLayout* GetInputLayout()const{ return m_inputLayout; }
	Shader* GetShader()const { return m_shader; }

	virtual ~TsShader()
	{
		TsSafeRelease( m_shader );
		TsSafeRelease( m_inputLayout );
	};
protected:
	Shader*				m_shader;
	ID3D11InputLayout*	m_inputLayout;
};

typedef TsShader<ID3D11VertexShader>	TsVertexShader;
typedef TsShader<ID3D11PixelShader>		TsPixelShader;
typedef TsShader<ID3D11GeometryShader>	TsGeometryShader;
typedef TsShader<ID3D11HullShader>		TsHullShader;
typedef TsShader<ID3D11DomainShader>	TsDomainShader;

typedef TsShader<ID3D11ComputeShader>	TsComputeShader;

// include inline template
#include "TsShader.cpp"