#include "../../../TsAfx.h"

TsTransformBakeAnimation::TsTransformBakeAnimation():
m_isLoop(TS_TRUE),
m_isRun(TS_TRUE),
m_localFrame(0),
m_maxFrame(0)
{

}
//==============================================================
// Set Local Frame 
//==============================================================
TsBool TsTransformBakeAnimation::SetLocalFrame( TsInt frame )
{
    m_localFrame = TsClamp( frame , 0 , m_maxFrame );
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
//    if( GetAsyncKeyState(VK_RIGHT) )
        m_localFrame+= 1.0f/30.0f;
//    if( GetAsyncKeyState( VK_LEFT ) )
//        m_localFrame--;
    if( m_localFrame < 0 )
        m_localFrame = m_maxFrame - 1;

    if( m_localFrame >= m_maxFrame )
        if( m_isLoop )
            m_localFrame = 0;
        else
            m_localFrame = m_maxFrame - 1;

    for each( auto p in m_pTransformList )
        if( p )
            *p = m_bakedMatrixList[m_localFrame].find( p->GetHashCode() )->second;

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