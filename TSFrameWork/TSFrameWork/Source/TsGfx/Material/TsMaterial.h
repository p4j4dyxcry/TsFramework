//**********************************************************
//! TsMaterial.h
//! material base.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsMaterial : public TsCBuffer
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsMaterial();
    virtual ~TsMaterial();
    virtual TsBool ApplyMaterial( TsDeviceContext* context ) ;
    virtual TsBool UpdateMaterial( TsDeviceContext* context );
    virtual TsBool CreateMaterial( TsDevice* pDevice );

protected:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    TsBool       m_isUpdate;
};