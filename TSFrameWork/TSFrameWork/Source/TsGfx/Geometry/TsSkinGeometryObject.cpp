#include"../TsGfx.h"

TsSkinGeometryObject::TsSkinGeometryObject() :
TsGeometryObject()
{

}
    
TsSkinGeometryObject::~TsSkinGeometryObject()
{
    TsSafeDelete( m_boneCBuffer );
}

TsBool TsSkinGeometryObject::CreateGeometryObject( TsDevice* pDev ,
                                               TsVertexElement * pMesh ,
                                               TsMaterial* pMaterial )
{
    TsGeometryObject::CreateGeometryObject(pDev, pMesh, pMaterial);
    m_boneCBuffer = TsNew( TsBoneCBuffer );
    m_boneCBuffer->CreateBoneCBuffer(pDev);
    return TS_TRUE;
}

TsBool TsSkinGeometryObject::UpdateTransform( TsDeviceContext* context )
{
    TsGeometryObject::UpdateTransform(context);

    if (m_boneCBuffer)
        m_boneCBuffer->UpdateCBuffer(context);
    else
        return TS_FALSE;

    return TS_TRUE;
}


TsBool TsSkinGeometryObject::ApplyTransForm( TsDeviceContext * context )
{
    TsGeometryObject::ApplyTransForm(context);
    if (m_boneCBuffer)
        m_boneCBuffer->ApplyCBuffer(context);
    else
        return TS_FALSE;
    return TS_TRUE;
}

TsBool TsSkinGeometryObject::SetSkeleton(TsSkeleton* TsSkeleton)
{
    m_boneCBuffer->SetSkeleton( TsSkeleton );
    return TS_TRUE;
}