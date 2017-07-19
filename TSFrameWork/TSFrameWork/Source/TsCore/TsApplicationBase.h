//**********************************************************
//! TsApplicationBase.h
//! App Base.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

class TsApplicationBase
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------    
    TsApplicationBase();
    virtual ~TsApplicationBase();

    TsBool Initialize( HINSTANCE hInstance , TsInt nWinMode );
    TsBool Run();
    TsDevice* GetDevice() const;
protected:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    TsDevice* m_pGfxDevice;
};