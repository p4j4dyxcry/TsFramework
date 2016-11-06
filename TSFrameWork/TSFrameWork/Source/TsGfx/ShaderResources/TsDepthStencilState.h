#pragma once

enum class TS_COMPARISON_FUNC : TsUint
{
    NEVER              = 1 ,   //&0 èÌÇ…ïsçáäi
    LESS                  = 2 ,   //<= É\Å[ÉXÇ™ëŒè€ÇÊÇËè¨Ç≥Ç¢Ç∆Ç´Ç…çáäi
    EQUAL              = 3 ,   //== ìôÇµÇ¢éûÇ…çáäi
    LESS_EQUAL = 4 ,   //<=  è¨Ç≥Ç¢Ç©ìôÇµÇ¢éûÇ…çáäi
    GREATER            = 5 ,   //>  É\Å[ÉXÇ™ëŒè€ÇÊÇËëÂÇ´Ç¢éûÇ…çáäi
    NOT_EQUAL          = 6 ,   //!= ìôÇµÇ≠Ç»Ç¢Ç∆Ç´Ç…çáäi
    GREATER_EQUAL      = 7 ,   //>= ëÂÇ´Ç¢Ç∆Ç´Ç©ìôÇµÇ¢éûÇ…çáäi
    ALWAYS             = 8 ,   //|1 èÌÇ…çáäi
};

class TsDepthStencilState
{
public:
    TsDepthStencilState();
    ~TsDepthStencilState();
    TsBool CreateDepthStencil( TsDevice * pDev );
    TsBool SetZEnable( TsBool flag);
    TsBool SetZWriteEnable( TsBool flag );
    TsBool SetDepthTestFunc( TS_COMPARISON_FUNC func );
    TsBool SetDepthTestFuncByString( const TsString& str );

    ID3D11DepthStencilState* GetDSS(){ return m_pDepteStencil; }
private:
    TsBool m_zRwriteEnable;
    TsBool m_zEnable;
    TS_COMPARISON_FUNC m_depthtTestFunc;
    ID3D11DepthStencilState* m_pDepteStencil;
};