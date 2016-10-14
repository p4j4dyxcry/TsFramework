#include "../../TsAfx.h"

TsDevice* TsResourceManager::m_pDevice = nullptr;
TsMap<TS_HASH , TsSamplerState*> TsResourceManager::m_SamplerLibrary;
TsMap<TS_HASH, TsMeshObject*> TsResourceManager::m_pMeshLibrary;
TsMap<TS_HASH, TsTexture2D*> TsResourceManager::m_FileTextureLibraty;

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
    m_SamplerLibrary.insert(pair);

    return TS_TRUE;
}

//-------------------------------------------------------------------------------
//  Find Resource Manager Template
//-------------------------------------------------------------------------------
template<class T>
TsBool TsResourceManaget_Register(TsMap<TS_HASH, T*>& map,
    T* pObj,
    const TsString& name)
{
    TS_HASH hash = TSUT::StringToHash(name);
    TsBool hResult = map.find(hash) == map.end();
    if (hResult == TS_TRUE)
    {
        std::pair<TS_HASH, T*> pair;
        pair.first = hash;
        pair.second = pObj;

        map.insert(pair);
    }

    return hResult;
}

//-------------------------------------------------------------------------------
//  Register Resource Manager Template
//-------------------------------------------------------------------------------
template<class T>
T* TsResourceManaget_Find(TsMap<TS_HASH, T*>& map,
                              const TsString& name)
{
    TS_HASH hash = TSUT::StringToHash(name);

    auto it = map.find(hash);
    if (it == map.end())
        return nullptr;

    return it->second;
}

//-------------------------------------------------------------------------------
//  Find No Type
//-------------------------------------------------------------------------------
template<typename T>
T* TsResourceManager::Find( const TsString& name )
{
    (void)name;
    return nullptr;
}

//-------------------------------------------------------------------------------
//  Find Sampler
//-------------------------------------------------------------------------------
template<>
TsSamplerState* TsResourceManager::Find(const TsString& name)
{
    return TsResourceManaget_Find(m_SamplerLibrary,name);
}

//-------------------------------------------------------------------------------
//  Find Mesh
//-------------------------------------------------------------------------------
template<>
TsMeshObject* TsResourceManager::Find(const TsString& name)
{
    return TsResourceManaget_Find(m_pMeshLibrary, name);
}

//-------------------------------------------------------------------------------
//  Find File Texture
//-------------------------------------------------------------------------------
template<>
TsTexture2D* TsResourceManager::Find(const TsString& name)
{
    return TsResourceManaget_Find(m_FileTextureLibraty, name);
}

//-------------------------------------------------------------------------------
//  Register No Type
//-------------------------------------------------------------------------------
template<typename T>
TsBool TsResourceManager::RegisterResource(T* pObject, const TsString& name)
{
    (void)pObject;
    (void)name;
    return TS_FALSE;
}

//-------------------------------------------------------------------------------
//  Register Sampler
//-------------------------------------------------------------------------------
template<>
TsBool TsResourceManager::RegisterResource(TsSamplerState* pObject, const TsString& name)
{
    return TsResourceManaget_Register(m_SamplerLibrary, pObject, name);
}

//-------------------------------------------------------------------------------
//  Register Mesh
//-------------------------------------------------------------------------------
template<>
TsBool TsResourceManager::RegisterResource(TsMeshObject* pObject, const TsString& name)
{
    return TsResourceManaget_Register(m_pMeshLibrary, pObject, name);
}


//-------------------------------------------------------------------------------
//  Register Texture
//-------------------------------------------------------------------------------
template<>
TsBool TsResourceManager::RegisterResource(TsTexture2D* pObject, const TsString& name)
{
    return TsResourceManaget_Register(m_FileTextureLibraty,pObject, name);
}