#include "TsGfx.h"

//! 継承して利用
TsBool TsDrawObject::Create( TsDevice* pDev)
{
    ( void )pDev;
    return TS_FALSE;
}

//! 継承して利用
TsBool TsDrawObject::Draw( TsDeviceContext* context)
{
    ( void )context;
    return TS_FALSE;
}

//! 継承して利用
TsBool TsDrawObject::UpdateTransform( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//! 継承して利用
TsBool TsDrawObject::UpdateMaterial( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//! 継承して利用
TsBool TsDrawObject::UpdateIndexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//! 継承して利用
TsBool TsDrawObject::UpdateVertexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//! 継承して利用
TsBool TsDrawObject::ApplyVertexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//! 継承して利用
TsBool TsDrawObject::ApplyTransForm( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//! 継承して利用
TsBool TsDrawObject::ApplyIndexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//! 継承して利用
TsBool TsDrawObject::ApplyMaterial( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}
//! 継承して利用
TsMatrix TsDrawObject::GetWorldMatrix()
{
    return TsMatrix::identity;
}

//! 継承して利用
TsMatrix TsDrawObject::GetLocalMatrix()
{
    return TsMatrix::identity;
}