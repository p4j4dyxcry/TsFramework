//!*******************************************************
//!	TsShaderEffect.h
//!
//!	Shader groups.
//!
//!	© 2016 Yuki Tsuneyama
#pragma once

class TsShaderEffect
{
public:
	//! Constructor
	TsShaderEffect() :	m_vertexShader(nullptr),
						m_pixelShader(nullptr),
						m_geometoryShader( nullptr ) ,
						m_hullShader( nullptr ) ,
						m_domainShader( nullptr ) ,
						m_computeShader(nullptr){}

	TsBool SetTexture( TsDeviceContext dc,
					   TsTexture* texture, 
					   TS_SHADER_TYPE target = TS_SHADER_TYPE::ALL_SHADER )
	{
		if( TSShaderUT::IsVS( target ) )
		{
//			dc.
//			m_vertexShader->GetShader()->
		}
	}

	TsVertexShader*		GetVertexShader()const{return m_vertexShader;}
	TsPixelShader*		GetPixelShader()const{ return m_pixelShader; }
	TsGeometryShader*	GetGeometryShader()const{ return m_geometoryShader; }
	TsHullShader*		GetHullShader()const{ return m_hullShader; }
	TsDomainShader*		GetDomainShader()const{ return m_domainShader; }
	TsComputeShader*	GetComputeShader()const{ return m_computeShader; }
protected:
	TsVertexShader *	m_vertexShader;
	TsPixelShader *		m_pixelShader;
	TsGeometryShader*	m_geometoryShader;
	TsHullShader *		m_hullShader;
	TsDomainShader *	m_domainShader;
	TsComputeShader *	m_computeShader;
};