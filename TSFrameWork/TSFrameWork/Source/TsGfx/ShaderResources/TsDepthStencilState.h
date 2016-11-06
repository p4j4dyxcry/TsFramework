#pragma once

enum class TS_COMPARISON_FUNC : TsUint
{
    NEVER              = 1 ,   //&0 ��ɕs���i
    LESS                  = 2 ,   //<= �\�[�X���Ώۂ�菬�����Ƃ��ɍ��i
    EQUAL              = 3 ,   //== ���������ɍ��i
    LESS_EQUAL = 4 ,   //<=  �����������������ɍ��i
    GREATER            = 5 ,   //>  �\�[�X���Ώۂ��傫�����ɍ��i
    NOT_EQUAL          = 6 ,   //!= �������Ȃ��Ƃ��ɍ��i
    GREATER_EQUAL      = 7 ,   //>= �傫���Ƃ������������ɍ��i
    ALWAYS             = 8 ,   //|1 ��ɍ��i
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