#include "TsFbxAfx.h"

TsFbxMaterial::TsFbxMaterial(TsFbxContext* pFbxContext, TsFbxScene* pFbxScene)
: TsFbxObject(pFbxContext, pFbxScene)
{
    m_specularPower =
    m_shininess		= 
    m_reflectivity	= 0.0f;
}

//! FbxMaterial を解析する
TsBool TsFbxMaterial::AnalizeForFbxMaterial( FbxSurfaceMaterial* pFbxMaterial )
{
    SetName( pFbxMaterial->GetName() );
    // ! ==step1 マテリアルがシェーダによって定義されているかを調べる。

    TsString lImplemenationType = "";
    const FbxImplementation* pFbxImplementation = GetImplementation( pFbxMaterial , FBXSDK_IMPLEMENTATION_HLSL );
    //! hlslで定義されている場合
    if( pFbxImplementation )
    {
        lImplemenationType = "HLSL";
    }
    //! cgで定義されている場合
    else if( !pFbxImplementation )
    {
        pFbxImplementation = GetImplementation( pFbxMaterial , FBXSDK_IMPLEMENTATION_CGFX );
        lImplemenationType = "CGFX";
    }

    //! シェーダで定義されている場合
    if( pFbxImplementation )
    {
        AnalizeCustomMaterial( pFbxMaterial , pFbxImplementation );
    }

    //! 通常のFbxSurfaceMaterial
    else
    {
        AnalizeDefaultMaterial( pFbxMaterial );
    }

    return TS_TRUE;
}

//シェーダで定義されているマテリアルから情報を取得
TsBool TsFbxMaterial::AnalizeCustomMaterial( FbxSurfaceMaterial* pFbxMaterial ,
                                             const FbxImplementation* pFbxImplementation )
{
    FbxProperty pPropery;

    const FbxBindingTable* lRootTable = pFbxImplementation->GetRootTable();
    const FbxBindingTable* lTable = pFbxImplementation->GetRootTable();
    size_t lEntryNum = lTable->GetEntryCount();
    for( TsInt i = 0; i < ( int )lEntryNum; ++i )
    {
        const FbxBindingTableEntry& lEntry = lTable->GetEntry( i );
        const char* lEntrySrcType = lEntry.GetEntryType( TS_TRUE );

        FbxString pEntrySource = lEntry.GetSource();

        if( strcmp( FbxPropertyEntryView::sEntryType , lEntrySrcType ) == 0 )
        {
            pPropery = pFbxMaterial->FindPropertyHierarchical( lEntry.GetSource() );
            if( !pPropery.IsValid() )
            {
                pPropery = pFbxMaterial->RootProperty.FindHierarchical( lEntry.GetSource() );
            }
        }
        else if( strcmp( FbxConstantEntryView::sEntryType , lEntrySrcType ) == 0 )
        {
            pPropery = pFbxImplementation->GetConstants().FindHierarchical( lEntry.GetSource() );
        }
        if( pPropery.IsValid() )
        {
            if( pPropery.GetSrcObjectCount<FbxTexture>() > 0 )
            {
                for( int j = 0; j < pPropery.GetSrcObjectCount<FbxFileTexture>(); ++j )
                {
                    FbxFileTexture *pTex = pPropery.GetSrcObject<FbxFileTexture>( j );
                    TextureType texType = TextureType::Albedo;

                    if( pEntrySource == "Maya|DiffuseTexture" )
                        texType = TextureType::Albedo;
                    else if( pEntrySource == "Maya|NormalTexture" )
                        texType = TextureType::Normal;
                    else if( pEntrySource == "Maya|SpecularTexture" )
                        texType = TextureType::Specular;
                    else if( pEntrySource == "Maya|FalloffTexture" )
                        texType = TextureType::Shininess;
                    else if( pEntrySource == "Maya|ReflectionMapTexture" )
                        texType = TextureType::Reflection;

                    m_texturename[(TsUint)texType][j].Analize( pTex->GetFileName() );
                }
            }
        }
    }

    return TS_TRUE;
}

//! 通常のfbxマテリアルから情報を取得
TsBool TsFbxMaterial::AnalizeDefaultMaterial( FbxSurfaceMaterial* pFbxMaterial )
{
    FbxProperty pPropery;

    // albedo
    pPropery = pFbxMaterial->FindProperty( FbxSurfaceMaterial::sDiffuse );
    AnalizeTextureName( pPropery , TextureType::Albedo );

    // spc
    pPropery = pFbxMaterial->FindProperty( FbxSurfaceMaterial::sSpecular );
    AnalizeTextureName( pPropery , TextureType::Specular );

    // amb
    pPropery = pFbxMaterial->FindProperty( FbxSurfaceMaterial::sAmbient );
    AnalizeTextureName( pPropery , TextureType::Amb );

    // normal
    pPropery = pFbxMaterial->FindProperty( FbxSurfaceMaterial::sNormalMap );
    AnalizeTextureName( pPropery , TextureType::Normal );

    //refrection
    pPropery = pFbxMaterial->FindProperty( FbxSurfaceMaterial::sReflection );
    AnalizeTextureName( pPropery , TextureType::Reflection );

    //shininess
    pPropery = pFbxMaterial->FindProperty( FbxSurfaceMaterial::sShininess );
    AnalizeTextureName( pPropery , TextureType::Shininess );

    //! get material scaler propery
    if( pFbxMaterial->GetClassId().Is( FbxSurfaceLambert::ClassId ) ||
        pFbxMaterial->GetClassId().Is( FbxSurfacePhong::ClassId ) )
    {
        FbxSurfaceLambert* pLambertMaterial = static_cast< FbxSurfaceLambert* >( ( FbxSurfaceLambert * )pFbxMaterial );

        //! color
        for( TsInt i = 0; i < 3; ++i )
        {
            m_diffuse[i]  = static_cast<TsF32>( pLambertMaterial->Diffuse.Get()[i] );
            m_ambient[i]  = static_cast<TsF32>( pLambertMaterial->Ambient.Get()[i] );
            m_emissive[i] = static_cast<TsF32>( pLambertMaterial->Emissive.Get()[i] );
        }
        //! alpha
        m_diffuse[3] = static_cast<TsF32>( pLambertMaterial->TransparencyFactor.Get() );

        //! phong material
        if( pFbxMaterial->GetClassId().Is( FbxSurfacePhong::ClassId ) )
        {
            FbxSurfacePhong* pPhongMaterial = static_cast< FbxSurfacePhong* >( ( FbxSurfacePhong * )pFbxMaterial );

            //! color
            for( TsInt i = 0; i < 3; ++i )
            {
                m_specular[i] = static_cast<TsF32>( pPhongMaterial->Specular.Get()[i] );
            }
            m_specularPower = 
            m_shininess	    = static_cast<TsF32>( pPhongMaterial->Shininess.Get() );
            m_reflectivity  = static_cast<TsF32>( pPhongMaterial->ReflectionFactor.Get() );

        }
    }
    
    return TS_TRUE;
}

//! プロパティからテクスチャパスの取得
TsBool TsFbxMaterial::AnalizeTextureName( const FbxProperty& pFbxPropery ,
                                          TextureType type )
{
    TsInt lLayeredTextureCount = pFbxPropery.GetSrcObjectCount<FbxLayeredTexture>();
    //! レイヤードテクスチャの対応最大３２枚
    if( lLayeredTextureCount > 0 )
    {
        for( TsInt j = 0; j<lLayeredTextureCount; ++j )
        {
            FbxFileTexture *pFbxLayeredTexture = pFbxPropery.GetSrcObject<FbxFileTexture>( j );
            TsInt lNbTextures = pFbxLayeredTexture->GetSrcObjectCount<FbxFileTexture>();

            m_texturename[static_cast<TsInt>(type)][j].Analize( pFbxLayeredTexture->GetFileName() );
            m_layersCount[static_cast<TsInt>( type )]++;
        }
    }
    //! 通常のテクスチャの対応
    else
    {
        TsInt lNbTextures = pFbxPropery.GetSrcObjectCount<FbxTexture>();

        if( lNbTextures > 0 )
        {
            for( TsInt j = 0; j<lNbTextures; ++j )
            {
                FbxFileTexture* pFbxTexture = pFbxPropery.GetSrcObject<FbxFileTexture>( j );
                if( pFbxTexture )
                {
                    m_texturename[static_cast<TsInt>( type )][0].Analize( pFbxTexture->GetFileName() );
                }
            }
        }
    }
    return TS_TRUE;
}

TSUT::TsFilePathAnalyzer TsFbxMaterial::GetTextureName( TextureType type , TsInt layer )const
{
    return m_texturename[static_cast< TsInt >( type )][layer];
}


TSUT::TsFilePathAnalyzer TsFbxMaterial::GetAlbedoTextureName( TsInt layer )const
{
    return GetTextureName(TextureType::Albedo,layer);
}
TSUT::TsFilePathAnalyzer TsFbxMaterial::GetSpecularTextureName( TsInt layer)const
{
    return GetTextureName( TextureType::Specular , layer );
}
TSUT::TsFilePathAnalyzer TsFbxMaterial::GetAmbientTextureName( TsInt layer )const
{
    return GetTextureName( TextureType::Amb , layer );
}
TSUT::TsFilePathAnalyzer TsFbxMaterial::GetNormalTextureName( TsInt layer )const
{
    return GetTextureName( TextureType::Normal , layer );
}
TSUT::TsFilePathAnalyzer TsFbxMaterial::GetReflectionTextureName( TsInt layer  )const
{
    return GetTextureName( TextureType::Reflection , layer );
}
TSUT::TsFilePathAnalyzer TsFbxMaterial::GetShininessTextureName( TsInt layer )const
{
    return GetTextureName( TextureType::Shininess , layer );
}

TsInt TsFbxMaterial::GetLayersCount( TextureType type )
{
    if( type >= TextureType::TextureTypeNum )
        return 0;
    return m_layersCount[static_cast< TsInt >( type )];
}