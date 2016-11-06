#include "../../../TsAfx.h"
TsMaterial::TsMaterial():
m_pTexture(nullptr),
m_isUpdate(TS_FALSE)
{}

TsMaterial::~TsMaterial()
{
    TsSafeDelete( m_pTexture );
}

TsBool TsMaterial::ApplyMaterial( TsDeviceContext* context )
{
    (void)context;
    return TS_TRUE;
}

TsBool TsMaterial::CreateMaterial( TsDevice * pDev )
{
    ( void )pDev;
    return TS_TRUE;
}

TsBool TsMaterial::UpdateMaterial( TsDeviceContext* context )
{
    ( void )context;
    return TS_TRUE;
}

TsBool TsMaterial::LoadTextureFromFile( TsDevice* pDev )
{
    m_pTexture = TsResourceManager::Find<TsTexture>(m_textureName);
    if (m_pTexture)
        m_pTexture->SetName(m_textureName);
    else
    {
        m_pTexture = TsNew(TsTexture2D);
        m_pTexture->SetName(m_textureName);
        auto pResult = TsDirectXTex::LoadFromFile(pDev->GetDevD3D(), m_textureName.c_str());
        if( pResult.pSrv )
        {
            m_pTexture->SetSRV( pResult.pSrv );
            m_pTexture->SetAlphaMode( pResult.IsAlphaEnable );
        }
        else
            return TS_FALSE;
        TsResourceManager::RegisterResource<TsTexture>(m_pTexture, m_textureName);
        return TS_TRUE;
    }
    return TS_TRUE;
}

TsTexture* TsMaterial::GetTexture()const
{
    return m_pTexture;
}