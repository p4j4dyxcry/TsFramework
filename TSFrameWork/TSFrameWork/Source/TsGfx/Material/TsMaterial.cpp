#include "../TsGfx.h"
#include <locale>
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
	m_pTexture= TsNew( TsTexture2D );
	m_pTexture->SetName( m_textureName );

	ID3D11ShaderResourceView* pSRV = TsDirectXTex::LoadFromFile( pDev->GetDevD3D() , m_name.c_str() );
	if( pSRV )
		m_pTexture->SetSRV( pSRV );
	else
		return TS_FALSE;

	return TS_TRUE;
}

TsTexture* TsMaterial::GetTexture()const
{
	return m_pTexture;
}