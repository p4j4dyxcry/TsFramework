//**********************************************************
//! TsResourceManager.h
//! refrence.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

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

    //! Find
    template<typename T>
    static T* Find(const TsString& name);

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

    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------        
    static TsDevice* m_pDevice;
    
    static TsMap<TS_HASH , TsSamplerState*> m_samplerLibrary;
    static TsMap<TS_HASH , TsMeshObject*>   m_meshLibrary;
    static TsMap<TS_HASH , TsTexture*>      m_fileTextureLibray;
    static TsMap<TS_HASH , TsSkeleton*>     m_skeletonLibrary;
    static TsMap<TS_HASH ,TsBlendState*>    m_blendStateLibrary;
};