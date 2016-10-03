#pragma once

class TsFbxMaterial :	public TsFbxObject ,
						public TsNameObject
{
public:
	enum TextureType
	{
		Albedo ,
		Spc ,
		Amb ,
		Normal ,
		Ref ,
		Shininess ,
		TextureTypeNum
	};

	TsFbxMaterial( TsFbxContext* pFbxContext );
	TsBool AnalizeForFbxMaterial( FbxSurfaceMaterial* pFbxMaterial );
	TSUT::TsFilePathAnalyzer	GetAlbedoTextureName(TsInt layer=0)const;
	TSUT::TsFilePathAnalyzer	GetSpecularTextureName( TsInt layer = 0 )const;
	TSUT::TsFilePathAnalyzer	GetAmbientTextureName( TsInt layer = 0 )const;
	TSUT::TsFilePathAnalyzer	GetNormalTextureName( TsInt layer = 0 )const;
	TSUT::TsFilePathAnalyzer	GetReflectionTextureName( TsInt layer = 0 )const;
	TSUT::TsFilePathAnalyzer	GetShininessTextureName( TsInt layer = 0 )const;

	TSUT::TsFilePathAnalyzer	GetTextureName( TextureType type , TsInt layer = 0 )const;

	TsInt GetLayersCount( TextureType type );
	static const TsInt MAX_LAYER = 32;
	
private:
	TsBool AnalizeTextureName(const FbxProperty & pFbxProperty ,
							   TextureType );

	TSUT::TsFilePathAnalyzer m_texturename[TextureType::TextureTypeNum][MAX_LAYER];
	TsInt	 m_layersCount[TextureType::TextureTypeNum];
	TsFloat4 m_albedo;
	TsFloat4 m_specular;
	TsFloat4 m_ambient;
	TsF32	 m_specularPower;
	TsF32	 m_Shininess;
};