//todo

#pragma once
class TsResourceManager 
{
public:
    static TsBool Initialize( TsDevice * pDev );

    static TsSamplerState* GetSamplerState( const TsString& );

private:
    static TsBool InitializeSampler( TsDevice* pDev );

    static TsDevice* m_pDevice;
    static TsMap<TS_HASH , TsSamplerState*> m_SamplerLibrary;
};