#include "../../../TsAfx.h"

TsDefaultMaterial::TsDefaultMaterial() 
{
    m_material.diffuse = TsFloat4( 1 , 1 , 1 , 1 );
    m_material.specluar = TsFloat4( 1 , 1 , 1 , 0 );
    m_material.ambient = TsFloat4( .5 , .5 , .5 , .5 );
    m_material.emissive = TsFloat3( 0 , 0 , 0 );
    m_material.mataric = 0;
    m_material.roughness = 0;

    m_material.useDiffuseMap = 0;
    m_material.useNomalMap = 0;
    m_material.useSpeculerMap = 0;

    m_pAlbedoTexture = nullptr;
    m_pNormalTexture = nullptr;
    m_pSpeclurTexture = nullptr;
};

TsDefaultMaterial::~TsDefaultMaterial()
{
};

TsBool TsDefaultMaterial::CreateMaterial( TsDevice* pDev )
{
    ID3D11Buffer* buffer = pDev->CreateBuffer( &m_material , sizeof( m_material ) , D3D11_CPU_ACCESS_WRITE , D3D11_BIND_CONSTANT_BUFFER );
    this->SetD3DBufferAndSize( buffer , sizeof( m_material ) );

    SetRegisterIndex(TS_CBUFFER_REGISTER::MaterialCB);  //register Index = [c1]
    BindShaderType( TS_SHADER_TYPE::PIXEL_SHADER );
    return TS_TRUE;
}

TsBool TsDefaultMaterial::ApplyMaterial( TsDeviceContext* pContext)
{
    pContext->SetCBuffer( this );

    if( m_pAlbedoTexture )
        pContext->SetTexture( AlbedoTextureRegister, m_pAlbedoTexture, TS_SHADER_TYPE::PIXEL_SHADER);

    if( m_pNormalTexture )
        pContext->SetTexture( NormalTextureRegister , m_pNormalTexture , TS_SHADER_TYPE::PIXEL_SHADER );

    if( m_pSpeclurTexture )
        pContext->SetTexture( SpecuerTextureRegister , m_pSpeclurTexture , TS_SHADER_TYPE::PIXEL_SHADER );

    pContext->SetSamplerState( TsResourceManager::Find<TsSamplerState>( TS_HASH_Default) , 0 );
    pContext->SetSamplerState( TsResourceManager::Find<TsSamplerState>( TS_HASH_Shadow), 3);
    pContext->SetSamplerState( TsResourceManager::Find<TsSamplerState>( TS_HASH_Cube), 4);
    return TS_TRUE;
}

TsBool TsDefaultMaterial::UpdateMaterial( TsDeviceContext* pContext )
{
	if( m_isUpdate )
		pContext->ChangeCBuffer( this , &m_material , GetBufferSize() );
    m_isUpdate = TS_FALSE;
    return TS_TRUE;
}

TsTexture* TsDefaultMaterial::LoadTextureFromFile( TsDevice* pDev ,const TsString& name)
{
    TsTexture * pTex = nullptr;
    pTex = TsResourceManager::Find<TsTexture>( name );
    if( pTex )
        pTex->SetName( name );
    else
    {
        auto pResult = TsDirectXTex::LoadFromFile( pDev->GetDevD3D() , name.c_str() );
        if( pResult.pSrv )
        {
            pTex = TsNew( TsTexture2D );
            pTex->SetName( name );

            pTex->SetSRV( pResult.pSrv );
            pTex->SetAlphaMode( pResult.IsAlphaEnable );
        }        
        TsResourceManager::RegisterResource<TsTexture>( pTex , name );
    }
    return pTex;
}

TsBool TsDefaultMaterial::LoadAlbedoTextureFromFile( TsDevice * pDev , const TsString& name )
{
    m_pAlbedoTexture = LoadTextureFromFile( pDev , name );

    TsBool useTexture = m_material.useDiffuseMap > 0.0f;
    if( m_pAlbedoTexture )
    {
        m_material.useDiffuseMap = 1.0f;
        m_isUpdate |= useTexture != TS_TRUE;
    }
    else
    {
        m_material.useDiffuseMap = 0;
        m_isUpdate |= useTexture != TS_FALSE;
    }
    return TS_TRUE;
}

TsBool TsDefaultMaterial::LoadNormalTextureFromFile( TsDevice * pDev , const TsString& name )
{
    m_pNormalTexture = LoadTextureFromFile( pDev , name );

    TsBool useTexture = m_material.useNomalMap > 0.0f;
    if( m_pNormalTexture )
    {
        m_material.useNomalMap = 1.0f;
        m_isUpdate |= useTexture != TS_TRUE;
    }
    else
    {
        m_material.useNomalMap = 0;
        m_isUpdate |= useTexture != TS_FALSE;
    }
    return TS_TRUE;
}

TsBool TsDefaultMaterial::LoadSpeculerTextureFromFile( TsDevice * pDev , const TsString& name )
{
    m_pSpeclurTexture = LoadTextureFromFile( pDev , name );

    TsBool useTexture = m_material.useSpeculerMap > 0.0f;
    if( m_pSpeclurTexture )
    {
        m_material.useSpeculerMap = 1.0f;
        m_isUpdate |= useTexture != TS_TRUE;
    }
    else
    {
        m_material.useSpeculerMap = 0;
        m_isUpdate |= useTexture != TS_FALSE;
    }
    return TS_TRUE;
}

TsBool TsDefaultMaterial::SetAlbedoTexture( TsTexture* pTex )
{
    m_pAlbedoTexture = pTex;
	if( pTex != nullptr )
		m_material.useDiffuseMap = 1.0f;
    m_isUpdate = TS_TRUE;
    return TS_TRUE;
}
TsBool TsDefaultMaterial::SetNormalTexture( TsTexture* pTex )
{
    m_pNormalTexture = pTex;
	if( pTex != nullptr )
		m_material.useNomalMap = 1.0f;
    m_isUpdate = TS_TRUE;
    return TS_TRUE;

}
TsBool TsDefaultMaterial::SetSpeculerTexture( TsTexture* pTex )
{
	if( pTex != nullptr )
		m_material.useSpeculerMap = 1.0f;
    m_pSpeclurTexture = pTex;
    m_isUpdate = TS_TRUE;
    return TS_TRUE;

}

TsBool TsDefaultMaterial::SetColor( const TsFloat4& color )
{
    m_material.diffuse = color;
    m_isUpdate = TS_TRUE;
    return TS_TRUE;
}
TsBool TsDefaultMaterial::SetAmbient( const TsFloat4& color )
{
    m_material.ambient = color;
    m_isUpdate = TS_TRUE;
    return TS_TRUE;
}
TsBool TsDefaultMaterial::SetEmissive( const TsFloat3& color )
{
    m_material.emissive[0] = color[0];
    m_material.emissive[1] = color[1];
    m_material.emissive[2] = color[2];
    m_isUpdate = TS_TRUE;
    return TS_TRUE;
}
TsBool TsDefaultMaterial::SetSpeculer( const TsFloat3& color )
{
    m_material.specluar[0] = color[0];
    m_material.specluar[1] = color[1];
    m_material.specluar[2] = color[2];
    m_isUpdate = TS_TRUE;
    return TS_TRUE;
}
TsBool TsDefaultMaterial::SetSpeculerPower( TsF32 power)
{
    m_material.specluar[3] = power;
    m_isUpdate = TS_TRUE;
    return TS_TRUE;
}
TsBool TsDefaultMaterial::SetMataric( TsF32 mataric )
{
    m_material.mataric = mataric;
    m_isUpdate = TS_TRUE;
    return TS_TRUE;
}
TsBool TsDefaultMaterial::SetRoughness( TsF32 roughness )
{
    m_material.roughness = roughness;
    m_isUpdate = TS_TRUE;
    return TS_TRUE;
}

TsTexture* TsDefaultMaterial::GetAlbedoTexture()const
{
    return m_pAlbedoTexture;
}
TsTexture* TsDefaultMaterial::GetNormalTexture()const
{
    return m_pNormalTexture;
}
TsTexture* TsDefaultMaterial::GetSpeculerTexture()const
{
    return m_pSpeclurTexture;
}

TsFloat4 TsDefaultMaterial::GetColor()const
{
    return m_material.diffuse;
}
TsFloat4 TsDefaultMaterial::GetAmbient()const
{
    return m_material.ambient;
}
TsFloat3 TsDefaultMaterial::GetEmissive()const
{
    return m_material.emissive;
}
TsFloat3 TsDefaultMaterial::GetSpecluer()const
{
    TsFloat4 c = m_material.specluar;
    return TsFloat3( c.x , c.y , c.z );
}
TsF32 TsDefaultMaterial::GetSpeculerPower()const
{
    return m_material.specluar.w;
}
TsF32 TsDefaultMaterial::GetMataric()const
{
    return m_material.mataric;
}
TsF32 TsDefaultMaterial::GetRoughness()const
{
    return m_material.roughness;
}