#include "../../TsAfx.h"

#define ASSIGN_INTERFACE( CLASS )\
TsMap< TS_HASH , CLASS *> TsResourceManager::m_##CLASS##Library;\
/*--------------------------------------*/\
/*Register Resource-of-name-------------*/\
/*--------------------------------------*/\
template<>\
TsBool TsResourceManager::RegisterResource( CLASS * p , const TsString& name)\
{\
    return TsResourceManager_Register<CLASS>( m_##CLASS##Library , p , name );\
}\
/*--------------------------------------*/\
/*Register Resource-of-hash-------------*/\
/*--------------------------------------*/\
template<>\
TsBool TsResourceManager::RegisterResource( CLASS * p , const TS_HASH hash)\
{\
    return TsResourceManager_Register<CLASS>( m_##CLASS##Library , p , hash );\
}\
/*--------------------------------------*/\
/*Find Resource-of-name-----------------*/\
/*--------------------------------------*/\
template<>\
CLASS * TsResourceManager::Find( const TsString& name)\
{\
    return TsResourceManager_Find( m_##CLASS##Library , name );\
}\
/*--------------------------------------*/\
/*Find Resource-of-hash-----------------*/\
/*--------------------------------------*/\
template<>\
CLASS * TsResourceManager::Find( const TS_HASH hash)\
{\
    return TsResourceManager_Find( m_##CLASS##Library , hash );\
}\

//-------------------------------------------------------------------------------
//  Register Resource Manager Template 
//-------------------------------------------------------------------------------
template<class T>
TsBool TsResourceManager_Register( TsMap<TS_HASH , T*>& map ,
                                   T* pObj ,
                                   const TS_HASH hash )
{
    TsBool hResult = map.find( hash ) == map.end();
    if( hResult == TS_TRUE )
    {
        std::pair<TS_HASH , T*> pair;
        pair.first = hash;
        pair.second = pObj;

        map.insert( pair );
    }

    return hResult;
}

//-------------------------------------------------------------------------------
//  Register Resource Manager Template 
//-------------------------------------------------------------------------------
template<class T>
TsBool TsResourceManager_Register( TsMap<TS_HASH , T*>& map ,
                                   T* pObj ,
                                   const TsString& name )
{
    TS_HASH hash = TSUT::StringToHash( name );

    return TsResourceManager_Register(map,pObj,hash);
}

//-------------------------------------------------------------------------------
//  Find Resource Manager Template
//-------------------------------------------------------------------------------
template<class T>
T* TsResourceManager_Find( TsMap<TS_HASH , T*>& map ,
                           const TS_HASH hash )
{
    auto it = map.find( hash );
    if( it == map.end() )
        return nullptr;

    return it->second;
}

//-------------------------------------------------------------------------------
//  Find Resource Manager Template
//-------------------------------------------------------------------------------
template<class T>
T* TsResourceManager_Find( TsMap<TS_HASH , T*>& map ,
                           const TsString& name )
{
    TS_HASH hash = TSUT::StringToHash( name );

    return TsResourceManager_Find(map,hash);
}

//-------------------------------------------------------------------------------
//  Register No Type
//-------------------------------------------------------------------------------
template<typename T>
TsBool TsResourceManager::RegisterResource( T* pObject , const TsString& name )
{
    TsDebugLogError("ResouceManagerへの追加失敗。型\"%s\"はサポートされていません。\n", typeid(T).name());
    ( void )pObject;
    ( void )name;
    return TS_FALSE;
}

//-------------------------------------------------------------------------------
//  Register No Type
//-------------------------------------------------------------------------------
template<typename T>
TsBool TsResourceManager::RegisterResource(T* pObject, const TS_HASH hash)
{
    TsDebugLogError("ResouceManagerへの追加失敗。型\"%s\"はサポートされていません。\n", typeid(T).name());
    (void)pObject;
    (void)hash;
    return TS_FALSE;
}

//-------------------------------------------------------------------------------
//  Find No Type
//-------------------------------------------------------------------------------
template<typename T>
T* TsResourceManager::Find( const TsString& name )
{
    ( void )name;
    return nullptr;
}

template<typename T>
T* TsResourceManager::Find(const TS_HASH hash)
{
    (void)name;
    return nullptr;
}

TsDevice* TsResourceManager::m_pDevice = nullptr;
//! ResourceManagerに新しい管理対象を追加する場合は

ASSIGN_INTERFACE(TsSamplerState)
ASSIGN_INTERFACE(TsMeshObject)
ASSIGN_INTERFACE(TsTexture)
ASSIGN_INTERFACE(TsSkeleton)
ASSIGN_INTERFACE(TsBlendState)
ASSIGN_INTERFACE(TsMaterial)

//-------------------------------------------------------------------------------
//! GlobalHash
//-------------------------------------------------------------------------------
#define INITIALIZE_PRE_HASH(name) (TS_HASH)TS_HASH_##name = TSUT::StringToHash(#name) ;

void InitializeGlobalHash()
{
    INITIALIZE_PRE_HASH(Default)
    INITIALIZE_PRE_HASH(nullptr)

    INITIALIZE_PRE_HASH(Cube)
    INITIALIZE_PRE_HASH(Shadow)

    INITIALIZE_PRE_HASH(NONE)
    INITIALIZE_PRE_HASH(ALPHA_BLEND)
    INITIALIZE_PRE_HASH(ADD)
    INITIALIZE_PRE_HASH(SUBTRACT)
}


TsBool TsResourceManager::Initialize( TsDevice* pDev )
{
    m_pDevice = pDev;
    InitializeGlobalHash();
    InitializeSampler( pDev );
    InitializeBlendState( pDev );
    InitializeMaterial(pDev);
    return TS_TRUE;
}

TsBool TsResourceManager::InitializeBlendState( TsDevice* pDev )
{
    //ブレンド無し
    {
        TsBlendState* pBlendState = nullptr;
        pBlendState = TsNew( TsBlendState );
        pBlendState->CreateBlendState( pDev , TS_BLEND_MODE::NONE );
        RegisterResource(pBlendState, TS_PRE_HASH(NONE));
    }

    //アルファブレンド
    {
        TsBlendState* pBlendState = nullptr;
        pBlendState = TsNew( TsBlendState );
        pBlendState->CreateBlendState( pDev , TS_BLEND_MODE::ALPHA_BLEND );
        RegisterResource(pBlendState, TS_PRE_HASH(ALPHA_BLEND));
    }

    //加算合成
    {
        TsBlendState* pBlendState = nullptr;
        pBlendState = TsNew( TsBlendState );
        pBlendState->CreateBlendState( pDev , TS_BLEND_MODE::ADD );
        RegisterResource(pBlendState, TS_PRE_HASH(ADD));
    }

    //減算合成
    {
        TsBlendState* pBlendState = nullptr;
        pBlendState = TsNew( TsBlendState );
        pBlendState->CreateBlendState( pDev , TS_BLEND_MODE::SUBTRACT );
        RegisterResource(pBlendState, TS_PRE_HASH(SUBTRACT));
    }

    return TS_TRUE;
}

TsBool TsResourceManager::InitializeSampler( TsDevice * pDev )
{
    //! Default Sampler    
    {
        D3D11_SAMPLER_DESC desc;

        desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;

        desc.BorderColor[0] =
        desc.BorderColor[1] =
        desc.BorderColor[2] =
        desc.BorderColor[3] = 0.0f;

        desc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;

        desc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
        desc.MaxAnisotropy = 16;
        desc.MinLOD = 0; //! Default Sampler    
        desc.MaxLOD = D3D11_FLOAT32_MAX;

        desc.MipLODBias = 0;

        TsSamplerState* pSamplerState = pDev->CreateSamplerState(desc);
        pSamplerState->SetName(TS_STR_Default);        
        RegisterResource(pSamplerState, TS_STR_Default);
    }

    //! Cube Map Sampler
    {
        D3D11_SAMPLER_DESC desc;

        desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;

        desc.BorderColor[0] =
        desc.BorderColor[1] =
        desc.BorderColor[2] =
        desc.BorderColor[3] = 0.0f;

        desc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;

        desc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        desc.MaxAnisotropy = 16;
        desc.MinLOD = 0;
        desc.MaxLOD = D3D11_FLOAT32_MAX;

        desc.MipLODBias = 0;
        TsSamplerState* pSamplerState = pDev->CreateSamplerState(desc);
        pSamplerState->SetName(TS_STR_Cube);
        RegisterResource(pSamplerState, TS_STR_Cube);
    }

    //! Shadow Sampler
    {
        D3D11_SAMPLER_DESC desc;
        ZeroMemory( &desc , sizeof( desc ) );
        desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;

        desc.BorderColor[0] = 1.0f;
        desc.BorderColor[1] = 1.0f;
        desc.BorderColor[2] = 1.0f;
        desc.BorderColor[3] = 1.0f;

        desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
        desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
        desc.MaxAnisotropy = 1;
        desc.MipLODBias = 0;
        desc.MinLOD = -FLT_MAX;
        desc.MaxLOD = +FLT_MAX;
        TsSamplerState* pSamplerState = pDev->CreateSamplerState(desc);
        pSamplerState->SetName(TS_STR_Shadow);
        RegisterResource(pSamplerState, TS_STR_Shadow);
    }
    return TS_TRUE;
}

TsBool TsResourceManager::InitializeMaterial(TsDevice * pDev)
{
    TsDefaultMaterial* pMaterial = TsNew(TsDefaultMaterial);
    pMaterial->CreateMaterial(pDev);
    pMaterial->SetName( TS_STR_nullptr );
    pMaterial->SetColor(TsFloat4(1, 0, 0.5, 1));

    RegisterResource<TsMaterial>(pMaterial, TS_STR_nullptr);

    return TS_TRUE;
}
