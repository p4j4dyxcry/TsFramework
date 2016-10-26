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
    TsBool SetFrameRate( TsF32 frameRate );
    TsBool SetTargetSkeleton( TsSkeleton* pSkeleton );
protected:
    TsBool m_isRun;
    TsBool m_isLoop;
    TsF32 m_localFrame;
    TsInt m_maxFrame;
    TsF32 m_frameRate;
    TsVector<TsMap<TS_HASH , TsMatrix>> m_bakedMatrixList;
    TsVector<TsTransForm*>  m_pTransformList;
    TsSkeleton* m_pTargetSkeleton;
};