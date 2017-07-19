#include "../../../TsAfx.h"

TsTransformBakeAnimation::TsTransformBakeAnimation():
m_isLoop(TS_TRUE),
m_isRun(TS_TRUE),
m_localFrame(0),
m_maxFrame(0),
m_frameRate(60.0f),
m_pTargetSkeleton(nullptr)
{

}
//==============================================================
// Set Local Frame 
//==============================================================
TsBool TsTransformBakeAnimation::SetLocalFrame( TsInt frame )
{
    m_localFrame = (TsF32)TsClamp( frame , 0 , m_maxFrame );
    return TS_TRUE;
}

//==============================================================
// Start Animation
//==============================================================
TsBool TsTransformBakeAnimation::Start()
{
    m_isRun = TS_TRUE;
    return TS_TRUE;
}

//==============================================================
// Update Transforms & local time
//==============================================================
TsBool TsTransformBakeAnimation::Update()
{
    if( m_isRun == TS_FALSE )
        return TS_TRUE;
    m_localFrame += 60.0f / 60.0f;

    if( m_localFrame < 0 )
        m_localFrame = m_maxFrame - 1.f;

    if( m_localFrame >= m_maxFrame )
        if( m_isLoop )
            m_localFrame = 0;
        else
            m_localFrame = m_maxFrame - 1.f;
    TsInt frame = ( TsInt )m_localFrame;
    for each(auto p in m_pTransformList)
    {
        if ( p )
        {
            TsTransForm begin =m_bakedMatrixList[frame].find( p->GetHashCode() )->second;
            //TsTransForm end;
            //if ( frame + 1 < m_maxFrame)
            //    end = m_bakedMatrixList[frame].find(p->GetHashCode())->second;
            //else 
            //    end = m_bakedMatrixList[0].find( p->GetHashCode() )->second;;
            TsF32 t = m_localFrame - static_cast<TsInt>(m_localFrame);

            //! この実装だとバグる。要調査
            // TsLerp( begin , end , 0 ).ToLocalMatrix();
            *p = begin.ToLocalMatrix();
        }
    }
    
    if (m_pTargetSkeleton)
    {
        m_pTargetSkeleton->ToExecutableState();
        m_pTargetSkeleton->UpdateSkeleton();
    }
    return TS_TRUE;
}

//==============================================================
// Stop Animation
//==============================================================
TsBool TsTransformBakeAnimation::Stop()
{
    m_isRun = TS_FALSE;
    return TS_TRUE;
}

//==============================================================
// Set Loop Flag
//==============================================================
TsBool TsTransformBakeAnimation::SetLoop( TsBool isLoop )
{
    m_isLoop = isLoop;
    return TS_TRUE;
}

//==============================================================
// Register compute transform target list
// :this method call after SetBakeAnimation
//==============================================================
TsBool TsTransformBakeAnimation::BindTransform( TsTransForm* pRoot )
{
    if( m_bakedMatrixList.empty() )
        return TS_FALSE;
    m_pTransformList.reserve( m_bakedMatrixList.size() );
    for( auto it = m_bakedMatrixList[0].begin(); it != m_bakedMatrixList[0].end(); ++it )
        m_pTransformList.push_back( pRoot->FindChildByhash( it->first ) );

    return TS_TRUE;
}

//==============================================================
// Set Bake Animations
//==============================================================
TsBool TsTransformBakeAnimation::SetBakeAnimation( const TsVector<TsMap<TS_HASH , TsMatrix>>& bakeAnim)
{
    m_bakedMatrixList = bakeAnim;
    m_maxFrame = m_bakedMatrixList.size();
    return TS_TRUE;
}

TsBool TsTransformBakeAnimation::SetFrameRate( TsF32 frameRate )
{
    m_frameRate = frameRate;
    return TS_TRUE;
}

TsBool TsTransformBakeAnimation::SetTargetSkeleton( TsSkeleton* pSkeleton )
{
    m_pTargetSkeleton = pSkeleton;
    return TS_TRUE;
}