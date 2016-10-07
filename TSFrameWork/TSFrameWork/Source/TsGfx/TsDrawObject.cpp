#include "TsGfx.h"

//! Œp³‚µ‚Ä—˜—p
TsBool TsDrawObject::Create( TsDevice* pDev)
{
    ( void )pDev;
    return TS_FALSE;
}

//! Œp³‚µ‚Ä—˜—p
TsBool TsDrawObject::Draw( TsDeviceContext* context)
{
    ( void )context;
    return TS_FALSE;
}

//! Œp³‚µ‚Ä—˜—p
TsBool TsDrawObject::UpdateTransform( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//! Œp³‚µ‚Ä—˜—p
TsBool TsDrawObject::UpdateMaterial( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//! Œp³‚µ‚Ä—˜—p
TsBool TsDrawObject::UpdateIndexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//! Œp³‚µ‚Ä—˜—p
TsBool TsDrawObject::UpdateVertexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//! Œp³‚µ‚Ä—˜—p
TsBool TsDrawObject::ApplyVertexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//! Œp³‚µ‚Ä—˜—p
TsBool TsDrawObject::ApplyTransForm( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//! Œp³‚µ‚Ä—˜—p
TsBool TsDrawObject::ApplyIndexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//! Œp³‚µ‚Ä—˜—p
TsBool TsDrawObject::ApplyMaterial( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}
//! Œp³‚µ‚Ä—˜—p
TsMatrix TsDrawObject::GetWorldMatrix()
{
    return TsMatrix::identity;
}

//! Œp³‚µ‚Ä—˜—p
TsMatrix TsDrawObject::GetLocalMatrix()
{
    return TsMatrix::identity;
}