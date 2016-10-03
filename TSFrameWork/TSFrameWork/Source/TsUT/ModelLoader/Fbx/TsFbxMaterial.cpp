#include "../../TsUT.h"
#include "TsFbxHeader.h"

TsFbxMaterial::TsFbxMaterial( TsFbxContext* pFbxContext )
	: TsFbxObject( pFbxContext )
{
	m_specularPower =
	m_Shininess = 0.0f;
}
TsBool TsFbxMaterial::AnalizeForFbxMaterial( FbxSurfaceMaterial* pFbxMaterial )
{
	FbxProperty pPropery;
	// albedo
	pPropery= pFbxMaterial->FindProperty( FbxSurfaceMaterial::sDiffuse );
	AnalizeTextureName( pPropery , TextureType::Albedo );

	// spc
	pPropery = pFbxMaterial->FindProperty( FbxSurfaceMaterial::sSpecular);
	AnalizeTextureName( pPropery , TextureType::Spc );

	// amb
	pPropery = pFbxMaterial->FindProperty( FbxSurfaceMaterial::sAmbient );
	AnalizeTextureName( pPropery , TextureType::Amb );

	// normal
	pPropery = pFbxMaterial->FindProperty( FbxSurfaceMaterial::sNormalMap );
	AnalizeTextureName( pPropery , TextureType::Normal );

	//refrection
	pPropery = pFbxMaterial->FindProperty( FbxSurfaceMaterial::sReflection );
	AnalizeTextureName( pPropery , TextureType::Ref );

	//shininess
	pPropery = pFbxMaterial->FindProperty( FbxSurfaceMaterial::sShininess );
	AnalizeTextureName( pPropery , TextureType::Shininess );

	return TS_TRUE;
}

TsBool TsFbxMaterial::AnalizeTextureName( const FbxProperty& pFbxPropery ,
										  TextureType type )
{
	TsInt lLayeredTextureCount = pFbxPropery.GetSrcObjectCount<FbxLayeredTexture>();
	if( lLayeredTextureCount > 0 )
	{
		for( TsInt j = 0; j<lLayeredTextureCount; ++j )
		{
			FbxFileTexture *lLayeredTexture = pFbxPropery.GetSrcObject<FbxFileTexture>( j );
			TsInt lNbTextures = lLayeredTexture->GetSrcObjectCount<FbxFileTexture>();

			m_texturename[static_cast<TsInt>(type)][j].Analize( lLayeredTexture->GetFileName() );
		}
	}
	else
	{
		TsInt lNbTextures = pFbxPropery.GetSrcObjectCount<FbxTexture>();

		if( lNbTextures > 0 )
		{
			for( TsInt j = 0; j<lNbTextures; ++j )
			{
				FbxFileTexture* lTexture = pFbxPropery.GetSrcObject<FbxFileTexture>( j );
				if( lTexture )
				{
					m_texturename[static_cast<TsInt>( type )][0].Analize( lTexture->GetFileName() );
				}
			}
		}
	}
	return TS_TRUE;
}

TSUT::TsFilePathAnalyzer	TsFbxMaterial::GetTextureName( TextureType type , TsInt layer )const
{
	return m_texturename[static_cast< TsInt >( type )][layer];
}


TSUT::TsFilePathAnalyzer	TsFbxMaterial::GetAlbedoTextureName( TsInt layer )const
{
	return GetTextureName(TextureType::Albedo,layer);
}
TSUT::TsFilePathAnalyzer	TsFbxMaterial::GetSpecularTextureName( TsInt layer)const
{
	return GetTextureName( TextureType::Spc , layer );
}
TSUT::TsFilePathAnalyzer	TsFbxMaterial::GetAmbientTextureName( TsInt layer )const
{
	return GetTextureName( TextureType::Amb , layer );
}
TSUT::TsFilePathAnalyzer	TsFbxMaterial::GetNormalTextureName( TsInt layer )const
{
	return GetTextureName( TextureType::Normal , layer );
}
TSUT::TsFilePathAnalyzer	TsFbxMaterial::GetReflectionTextureName( TsInt layer  )const
{
	return GetTextureName( TextureType::Ref , layer );
}
TSUT::TsFilePathAnalyzer	TsFbxMaterial::GetShininessTextureName( TsInt layer )const
{
	return GetTextureName( TextureType::Shininess , layer );
}

TsInt TsFbxMaterial::GetLayersCount( TextureType type )
{
	if( type >= TextureType::TextureTypeNum )
		return 0;
	return m_layersCount[static_cast< TsInt >( type )];
}