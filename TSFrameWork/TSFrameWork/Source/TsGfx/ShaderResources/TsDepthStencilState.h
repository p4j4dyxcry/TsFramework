//**********************************************************
//! TsDepthStencilState.h
//! Depth / Stencil State.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

//----------------------------------------------------------
// Define of Depth Test Function
//----------------------------------------------------------
enum class TS_COMPARISON_FUNC : TsUint
{
    NEVER              = 1 ,   //&0 常に不合格
    LESS               = 2 ,   //<= ソースが対象より小さいときに合格
    EQUAL              = 3 ,   //== 等しい時に合格
    LESS_EQUAL         = 4 ,   //<=  小さいか等しい時に合格
    GREATER            = 5 ,   //>  ソースが対象より大きい時に合格
    NOT_EQUAL          = 6 ,   //!= 等しくないときに合格
    GREATER_EQUAL      = 7 ,   //>= 大きいときか等しい時に合格
    ALWAYS             = 8 ,   //|1 常に合格
};

class TsDepthStencilState
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsDepthStencilState();
    virtual ~TsDepthStencilState();
    TsBool CreateDepthStencil( TsDevice * pDev );
    TsBool SetZEnable( TsBool flag);
    TsBool SetZWriteEnable( TsBool flag );
    TsBool SetDepthTestFunc( TS_COMPARISON_FUNC func );
    TsBool SetDepthTestFuncByString( const TsString& str );

    ID3D11DepthStencilState* GetDSS(){ return m_pDepteStencil; }
private:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    TsBool m_zRwriteEnable;
    TsBool m_zEnable;
    TS_COMPARISON_FUNC m_depthtTestFunc;
    ID3D11DepthStencilState* m_pDepteStencil;
};