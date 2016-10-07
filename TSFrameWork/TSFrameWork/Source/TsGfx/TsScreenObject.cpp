#include "TsGfx.h"

TsScreenObject::TsScreenObject() :
m_vertexBuffer( nullptr )
{
    m_vertex[0].data = TsVector4(-1 , 1 , 0 , 0 ) ;
    m_vertex[1].data = TsVector4( 1 , 1 , 1 , 0 ) ;
    m_vertex[2].data = TsVector4(-1 ,-1 , 0 , 1 ) ;
    m_vertex[3].data = TsVector4( 1 ,-1 , 1 , 1 ) ;
        
}

TsScreenObject::~TsScreenObject()
{
    TsSafeDelete( m_vertexBuffer );
}
TsBool TsScreenObject::Create( TsDevice* pDev )
{
    m_vertexBuffer = pDev->CreateVertexBuffer( m_vertex , sizeof( m_vertex ) , sizeof( TSVertexScreenSpace ) , 0 );
    return TS_TRUE;
}

TsBool TsScreenObject::Draw( TsDeviceContext* context )
{
    context->Draw( 4 , 0 );
    return TS_TRUE;
}

TsBool TsScreenObject::ApplyVertexBuffer( TsDeviceContext* context )
{
    context->SetVertexBuffer(m_vertexBuffer);
    
    return TS_TRUE;
}