#include "TsGfx.h"

//! �p�����ė��p
TsBool TsDrawObject::Create( TsDevice* pDev)
{
    ( void )pDev;
    return TS_FALSE;
}

//! �p�����ė��p
TsBool TsDrawObject::Draw( TsDeviceContext* context)
{
    ( void )context;
    return TS_FALSE;
}

//! �p�����ė��p
TsBool TsDrawObject::UpdateTransform( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//! �p�����ė��p
TsBool TsDrawObject::UpdateMaterial( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//! �p�����ė��p
TsBool TsDrawObject::UpdateIndexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//! �p�����ė��p
TsBool TsDrawObject::UpdateVertexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//! �p�����ė��p
TsBool TsDrawObject::ApplyVertexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//! �p�����ė��p
TsBool TsDrawObject::ApplyTransForm( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//! �p�����ė��p
TsBool TsDrawObject::ApplyIndexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//! �p�����ė��p
TsBool TsDrawObject::ApplyMaterial( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}
//! �p�����ė��p
TsMatrix TsDrawObject::GetWorldMatrix()
{
    return TsMatrix::identity;
}

//! �p�����ė��p
TsMatrix TsDrawObject::GetLocalMatrix()
{
    return TsMatrix::identity;
}