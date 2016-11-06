#pragma once

class TsResourceManager 
{
public:
    static TsBool Initialize( TsDevice * pDev );

    template<typename T> 
    static TsBool RegisterResource( T* pObject, const TsString& name);

    template<typename T>
    static T* Find(const TsString& name);

private:
    static TsBool InitializeSampler( TsDevice* pDev );
    static TsBool InitializeBlendState( TsDevice* pDev );

    static TsDevice* m_pDevice;
    static TsMap<TS_HASH , TsSamplerState*> m_samplerLibrary;
    static TsMap<TS_HASH , TsMeshObject*>   m_meshLibrary;
    static TsMap<TS_HASH, TsTexture*>       m_fileTextureLibray;
    static TsMap<TS_HASH , TsSkeleton*>     m_skeletonLibrary;
    static TsMap<TS_HASH,TsBlendState*>     m_blendStateLibrary;
};