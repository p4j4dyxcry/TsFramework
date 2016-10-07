#include "TsGfx.h"

TsDrawQueue::TsDrawQueue()
{
    m_drawObjList.reserve( 512 );
}

TsDrawQueue::~TsDrawQueue()
{
    m_drawObjList.clear();
}

TsBool TsDrawQueue::Add( TsDrawObject* obj )
{
    if( obj == nullptr )
        return TS_FALSE;
    m_drawObjList.push_back( obj );

    return TS_TRUE;
}
TsDrawObject* TsDrawQueue::FindGeometoryByName( TsString name )
{
    TS_HASH hash = TSUT::StringToHash( name );
    for( auto it : m_drawObjList )
        if( hash == it->GetHashCode() )
            return it;
    TsDebugLog( "Find Fail\n" );
    return nullptr;
}

TsDrawObject* TsDrawQueue::FindGeometoryByIndex(TsInt index)
{
    if (m_drawObjList.size() > (unsigned)index)
        return m_drawObjList[index];
    else
        return nullptr;
}
TsBool TsDrawQueue::Remove( TsDrawObject * obj )
{
    for( auto it = m_drawObjList.begin(); it != m_drawObjList.end(); ++it )
    {
        if( obj == *it )
        {
            m_drawObjList.erase( it );
            return TS_TRUE;
        }
    }
    TsDebugLog( "Remove Fail\n" );
    return TS_FALSE;
}

TsBool TsDrawQueue::Begin( TsDeviceContext * pDc ,
                           TsBool zEnable ,
                           TsBool zWriteEnable ,
                           D3D11_CULL_MODE cullMopde )
{
    //todo dc set data
    ( void )zEnable;
    ( void )zWriteEnable;
    ( void )cullMopde;

    return TS_TRUE;
}

TsBool TsDrawQueue::End()
{
    //todo
    return TS_TRUE;
}

TsBool TsDrawQueue::Render( TsDeviceContext* pDc)
{
    for each ( auto var in m_drawObjList )
    {
        var->UpdateIndexBuffer( pDc );
        var->UpdateVertexBuffer( pDc );
        var->UpdateMaterial( pDc );
        var->UpdateTransform( pDc );

        var->ApplyIndexBuffer( pDc );
        var->ApplyVertexBuffer( pDc );
        var->ApplyMaterial( pDc );
        var->ApplyTransForm( pDc );
        var->Draw(pDc);
    }
    return TS_TRUE;
}