#include "../../../TsAfx.h"
TsMaterial::TsMaterial():
m_isUpdate(TS_FALSE)
{}

TsMaterial::~TsMaterial()
{}

TsBool TsMaterial::ApplyMaterial( TsDeviceContext* context )
{
    (void)context;
    return TS_TRUE;
}

TsBool TsMaterial::CreateMaterial( TsDevice * pDev )
{
    ( void )pDev;
    return TS_TRUE;
}

TsBool TsMaterial::UpdateMaterial( TsDeviceContext* context )
{
    ( void )context;
    return TS_TRUE;
}
