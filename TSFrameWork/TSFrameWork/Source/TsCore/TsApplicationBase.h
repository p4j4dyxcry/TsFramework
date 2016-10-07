#pragma once

class TsApplicationBase
{
public:
    TsApplicationBase();

    TsBool Initialize( HINSTANCE hInstance , TsInt nWinMode );
    TsBool Run();
    TsDevice* GetDevice();
protected:
    TsDevice* m_pGfxDevice;
};