#include "../TsGfx.h"

//----------------------------------------------------------
//! �W�I���g�����쐬���܂��B
//  �p�����ė��p
//----------------------------------------------------------
TsBool TsDrawObject::Create( TsDevice* pDev)
{
    ( void )pDev;
    return TS_FALSE;
}

//----------------------------------------------------------
//! �I�u�W�F�N�g��DrawCall�ł��쐬���܂��B
//  �p�����ė��p
//----------------------------------------------------------
TsBool TsDrawObject::Draw( TsDeviceContext* context)
{
    ( void )context;
    return TS_FALSE;
}

//----------------------------------------------------------
//! �I�u�W�F�N�g��Transform���X�V���܂��B
//  �p�����ė��p
//----------------------------------------------------------
TsBool TsDrawObject::UpdateTransform( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//----------------------------------------------------------
//! �I�u�W�F�N�g��Material���X�V���܂��B
//  �p�����ė��p
//----------------------------------------------------------
TsBool TsDrawObject::UpdateMaterial( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//----------------------------------------------------------
//! �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@���X�V���܂��B
//  �p�����ė��p
//----------------------------------------------------------
TsBool TsDrawObject::UpdateIndexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//----------------------------------------------------------
//! �I�u�W�F�N�g�̒��_�o�b�t�@���X�V���܂��B
//  �p�����ė��p
//----------------------------------------------------------
TsBool TsDrawObject::UpdateVertexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//----------------------------------------------------------
//! �I�u�W�F�N�g�̒��_�o�b�t�@���m�肵�܂��B
//  �p�����ė��p
//----------------------------------------------------------
TsBool TsDrawObject::ApplyVertexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//----------------------------------------------------------
//! �I�u�W�F�N�g��Transofrom���m�肵�܂��B
//  �p�����ė��p
//----------------------------------------------------------
TsBool TsDrawObject::ApplyTransForm( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//----------------------------------------------------------
//! �I�u�W�F�N�g��IndexBuffer���m�肵�܂��B
//  �p�����ė��p
//----------------------------------------------------------
TsBool TsDrawObject::ApplyIndexBuffer( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//----------------------------------------------------------
//! �I�u�W�F�N�g��Material���m�肵�܂��B
//  �p�����ė��p
//----------------------------------------------------------
TsBool TsDrawObject::ApplyMaterial( TsDeviceContext* context )
{
    ( void )context;
    return TS_FALSE;
}

//----------------------------------------------------------
//! �I�u�W�F�N�g��WorldMatrix���m�肵�܂��B
//  �p�����ė��p
//----------------------------------------------------------
TsMatrix TsDrawObject::GetWorldMatrix()const
{
    return TsMatrix::identity;
}


//----------------------------------------------------------
//! �I�u�W�F�N�g��LocalMatrix���m�肵�܂��B
//  �p�����ė��p
//----------------------------------------------------------
TsMatrix TsDrawObject::GetLocalMatrix()const
{
    return TsMatrix::identity;
}