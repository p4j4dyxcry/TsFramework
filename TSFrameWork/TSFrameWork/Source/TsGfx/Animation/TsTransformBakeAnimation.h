#pragma once

class TsTransformBakeAnimation
{
public :
    TsTransformBakeAnimation();
    TsBool SetLocalFrame( TsInt frame );
    TsBool Start();
    TsBool Update();
    TsBool Stop();
    TsBool SetLoop( TsBool isLoop );
    TsBool BindTransform( TsTransForm* pRoot );
    TsBool SetBakeAnimation( const TsVector<TsMap<TS_HASH , TsMatrix>>& );
protected:
    TsBool m_isRun;
    TsBool m_isLoop;
    TsF32 m_localFrame;
    TsInt m_maxFrame;
    TsVector<TsMap<TS_HASH , TsMatrix>> m_bakedMatrixList;
    TsVector<TsTransForm*>  m_pTransformList;
};