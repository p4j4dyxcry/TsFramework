//**********************************************************
//! TsMaterial.h
//! material base.
//!
//! © 2016 Yuki Tsuneyama
#pragma once
class CBufferBase;

class TsMaterial : public CBufferBase
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
	TsMaterial()
	{
		m_isUpdate = TS_FALSE;
	};
	virtual TsBool TsMaterial::ApplyMaterial( TsDeviceContext* context )
	{
		( void )context;
		return TS_TRUE;
	}

	virtual TsBool TsMaterial::CreateMaterial( TsDevice * pDev )
	{
		( void )pDev;
		return TS_TRUE;
	}

	virtual TsBool TsMaterial::UpdateMaterial( TsDeviceContext* context )
	{
		( void )context;
		return TS_TRUE;
	}
protected:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    TsBool       m_isUpdate;
};