#include"../TsGfx.h"

TsSkinGeometryObject::TsSkinGeometryObject() :
TsGeometryObject()
{

}
    
TsSkinGeometryObject::~TsSkinGeometryObject()
{
    TsSafeDelete( m_boneCBuffer );
    TsSafeDelete( m_pdumyTranform );
}

TsBool TsSkinGeometryObject::CreateGeometryObject( TsDevice* pDev ,
                                               TsVertexElement * pMesh ,
                                               TsMaterial* pMaterial )
{
    TsGeometryObject::CreateGeometryObject(pDev, pMesh, pMaterial);
    m_boneCBuffer = TsNew( TsBoneCBuffer );
    m_boneCBuffer->CreateBoneCBuffer(pDev);

    //=========================================================================
    //! �X�L�����b�V����Transform���g�p���Ȃ��̂Ń_�~�[�Ƃ��ĒP�ʍs��𑗂�
    // ���@����Transform �̒l��ύX����ƊK�w�\���ɂ�
    //=========================================================================
    m_pdumyTranform = TsNew( TsCBuffer );

    TsMatrix identity = TsMatrix::identity;

    m_pdumyTranform->CreateCBuffer( 
        pDev ,&identity,
        TS_CBUFFER_REGISTER::InstanceCB);
    return TS_TRUE;
}

TsBool TsSkinGeometryObject::UpdateTransform( TsDeviceContext* context )
{
//    TsGeometryObject::UpdateTransform(context);

    if (m_boneCBuffer)
        m_boneCBuffer->UpdateCBuffer(context);
    else
        return TS_FALSE;

    return TS_TRUE;
}


TsBool TsSkinGeometryObject::ApplyTransForm( TsDeviceContext * context )
{
//    TsGeometryObject::ApplyTransForm(context);
    if( m_pdumyTranform )
        m_pdumyTranform->ApplyCBuffer( context );
    if (m_boneCBuffer)
        m_boneCBuffer->ApplyCBuffer(context);
    else
        return TS_FALSE;
    return TS_TRUE;
}

TsBool TsSkinGeometryObject::SetSkeleton(TsSkeleton* TsSkeleton)
{
    m_pSkeleton = TsSkeleton;
    m_boneCBuffer->SetSkeleton( TsSkeleton );
    return TS_TRUE;
}