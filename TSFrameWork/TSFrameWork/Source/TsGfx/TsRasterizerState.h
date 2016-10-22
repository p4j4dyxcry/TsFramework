#pragma once

enum TS_CULL_MODE
{
    BACK    = D3D11_CULL_BACK,
    FRONT   = D3D11_CULL_FRONT ,
    NONE    = D3D11_CULL_NONE
};

enum TS_FILL_MODE
{
    SOLID       = D3D11_FILL_MODE::D3D11_FILL_SOLID,
    WIREFRAME   = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME 
};


class TsRasterizerState
{
public:
    TsRasterizerState();

    TsBool SetCullMode( TS_CULL_MODE mode );
    TsBool SetFillMode( TS_FILL_MODE mode );

    TsBool SetCullModeByString( const TsString& mode );
    TsBool SetFillModeByString(  const TsString& mode );

    TsBool SetAntiAliasMode( TsBool flag );

    TsBool CreateRasterizerState( TsDevice* pDev );

    TsBool operator == ( const TsRasterizerState& rasterizer );

    TsBool operator != ( const TsRasterizerState& rasterizer );

    ID3D11RasterizerState* GetRSS(){ return m_pD3DRasterizerState; }
    TS_CULL_MODE GetCullMode(){ return m_cullMode; }
    TS_FILL_MODE GetFillMode(){ return m_fillMode; }

    TsBool       GetAntiAlias(){ return m_antiAlias; }
private:
    TS_CULL_MODE m_cullMode;
    TS_FILL_MODE m_fillMode;
    TsBool       m_antiAlias;
    ID3D11RasterizerState* m_pD3DRasterizerState;
};