#include "../../TsAfx.h"

#define ASSIGN_INTERFACE( T ,Name )\
TsMap<TS_HASH,T*> TsResourceManager::Name;\
/*--------------------------------------*/\
/*Register Resource---------------------*/\
/*--------------------------------------*/\
template<>\
TsBool TsResourceManager::RegisterResource( T* p , const TsString& name)\
{\
    return TsResourceManager_Register( Name , p , name );\
}\
/*--------------------------------------*/\
/*Find Resource-------------------------*/\
/*--------------------------------------*/\
template<>\
T* TsResourceManager::Find( const TsString& name)\
{\
    return TsResourceManager_Find( Name , name );\
}\

//-------------------------------------------------------------------------------
//  Find Resource Manager Template
//-------------------------------------------------------------------------------
template<class T>
TsBool TsResourceManager_Register( TsMap<TS_HASH , T*>& map ,
                                   T* pObj ,
                                   const TsString& name )
{
    TS_HASH hash = TSUT::StringToHash( name );
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
T* TsResourceManager_Find( TsMap<TS_HASH , T*>& map ,
                           const TsString& name )
{
    TS_HASH hash = TSUT::StringToHash( name );

    auto it = map.find( hash );
    if( it == map.end() )
        return nullptr;

    return it->second;
}

//-------------------------------------------------------------------------------
//  Register No Type
//-------------------------------------------------------------------------------
template<typename T>
static TsBool TsResourceManager::RegisterResource( T* pObject , const TsString& name )
{
    ( void )pObject;
    ( void )name;
    return TS_FALSE;
}

//-------------------------------------------------------------------------------
//  Find No Type
//-------------------------------------------------------------------------------
template<typename T>
static T* TsResourceManager::Find( const TsString& name )
{
    ( void )name;
    return nullptr;
}

TsBool TsResourceManager::Initialize( TsDevice* pDev )
{
    m_pDevice = pDev;
    InitializeSampler( pDev );

    return TS_TRUE;
}

TsBool TsResourceManager::InitializeSampler( TsDevice * pDev )
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
    desc.MinLOD = 0;
    desc.MaxLOD = D3D11_FLOAT32_MAX;

    desc.MipLODBias = 0;
    std::pair<TS_HASH , TsSamplerState*> pair( TSUT::StringToHash("Default") , pDev->CreateSamplerState( desc ) );

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
    std::pair<TS_HASH, TsSamplerState*> pair2(TSUT::StringToHash("Cube"), pDev->CreateSamplerState(desc));


    m_SamplerLibrary.insert(pair2);

    return TS_TRUE;
}




TsDevice* TsResourceManager::m_pDevice = nullptr;
ASSIGN_INTERFACE( TsSamplerState , m_SamplerLibrary );
ASSIGN_INTERFACE( TsMeshObject , m_pMeshLibrary );
ASSIGN_INTERFACE( TsTexture2D , m_FileTextureLibray );
ASSIGN_INTERFACE( TsSkeleton , m_pSkeletonLibrary );