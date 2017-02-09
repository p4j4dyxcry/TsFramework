//**********************************************************
//! TsResourceManager.h
//! refrence.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

namespace 
{
#define TS_REGISTER_PRE_HASH(name) static TS_HASH TS_HASH_##name ; static const TsChar* TS_STR_##name = #name ;
    TS_REGISTER_PRE_HASH(nullptr)
    TS_REGISTER_PRE_HASH(Default)
    TS_REGISTER_PRE_HASH(Cube)
    TS_REGISTER_PRE_HASH(NONE)
    TS_REGISTER_PRE_HASH(Shadow)
    TS_REGISTER_PRE_HASH(ALPHA_BLEND)
    TS_REGISTER_PRE_HASH(ADD)
    TS_REGISTER_PRE_HASH(SUBTRACT)
#undef TS_REGISTER_PRE_HASH
}
#define TS_PRE_HASH(name) ::TS_HASH_##name
#define TS_PRE_STR(name)  ::TS_STR_##name
#define TS_RESOURCE_MANAGER_ASSIGNE(CLASS) static TsMap<TS_HASH , CLASS *> m_##CLASS##Library ; 

class TsResourceManager 
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------

    //! Initalize
    static TsBool Initialize( TsDevice * pDev );

    //! Register
    template<typename T> 
    static TsBool RegisterResource( T* pObject, const TsString& name);

    //! Register
    template<typename T>
    static TsBool RegisterResource(T* pObject, const TS_HASH hash);

    //! Find
    template<typename T>
    static T* Find(const TsString& name);

    //! Find
    template<typename T>
    static T* Find(const TS_HASH hash);

    //! Release
    // : todo ここで解放する？

private:
    //----------------------------------------------------------
    // private method
    //----------------------------------------------------------
    // ! デフォルトサンプラの作成＆登録
    static TsBool InitializeSampler(TsDevice* pDev);

    // ! デフォルトブレンドステートの登録
    static TsBool InitializeBlendState(TsDevice* pDev);

    static TsBool InitializeMaterial(TsDevice* pDev);

    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------        
    static TsDevice* m_pDevice;
    TS_RESOURCE_MANAGER_ASSIGNE(TsSamplerState)
    TS_RESOURCE_MANAGER_ASSIGNE(TsMeshObject)
    TS_RESOURCE_MANAGER_ASSIGNE(TsMaterial)
    TS_RESOURCE_MANAGER_ASSIGNE(TsTexture)
    TS_RESOURCE_MANAGER_ASSIGNE(TsSkeleton)
    TS_RESOURCE_MANAGER_ASSIGNE(TsBlendState)
};