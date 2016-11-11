//!*******************************************************
//! TsShaderEffect.h
//!
//! Shader groups.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsShaderEffect :	public TsNameObject ,
                        public ITsStaticNameObjectList<TsShaderEffect>
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    //! Constructor
    TsShaderEffect() :	m_vertexShader(nullptr),
                        m_pPixelShader(nullptr),
                        m_pGeometoryShader( nullptr ) ,
                        m_pHullShader( nullptr ) ,
                        m_pDomainShader( nullptr ) ,
                        m_pComputeShader(nullptr),
                        m_pDepthStencilState(nullptr),
                        m_pRasterizerState(nullptr),
                        m_pBlensState(nullptr){}

    TsBool LoadPackageFromXml( TsDevice* pDev , const TsString& file );

    TsBool SetVertexShader( TsVertexShader* );
    TsBool SetPixelShader( TsPixelShader* );
    TsBool SetGeometryShader( TsGeometryShader* );
    TsBool SetHullShader( TsHullShader* );
    TsBool SetDomainShader( TsDomainShader* );
    TsBool SetComputeShader( TsComputeShader* );

    ID3D11InputLayout*  GetInputLayout();
    TsVertexShader*     GetVertexShader()const{return m_vertexShader;}
    TsPixelShader*      GetPixelShader()const{ return m_pPixelShader; }
    TsGeometryShader*   GetGeometryShader()const{ return m_pGeometoryShader; }
    TsHullShader*       GetHullShader()const{ return m_pHullShader; }
    TsDomainShader*     GetDomainShader()const{ return m_pDomainShader; }
    TsComputeShader*    GetComputeShader()const{ return m_pComputeShader; }	

    TsRasterizerState*   GetRasterizeState()      { return m_pRasterizerState; }
    TsDepthStencilState* GetDepthStencilState(){ return m_pDepthStencilState; }
    TsBlendState*        GetBlendState(){ return m_pBlensState; };

protected:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    TsVertexShader *    m_vertexShader;
    TsPixelShader *     m_pPixelShader;
    TsGeometryShader*   m_pGeometoryShader;
    TsHullShader *      m_pHullShader;
    TsDomainShader *    m_pDomainShader;
    TsComputeShader *   m_pComputeShader;

    TsDepthStencilState* m_pDepthStencilState;
    TsRasterizerState*   m_pRasterizerState;
    TsBlendState*        m_pBlensState;
};