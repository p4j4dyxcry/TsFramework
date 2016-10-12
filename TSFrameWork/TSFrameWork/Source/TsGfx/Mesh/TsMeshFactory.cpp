#include "../TsGfx.h"

TsMeshFactory::TsMeshFactory()
{

}
TsMeshFactory::~TsMeshFactory()
{

}

TsBool TsMeshFactory::LoadFromFile( TsDevice* pDev, TsString filename )
{
    if( filename.rfind( ".mqo" ) != TsString::npos )
    {
        TsMqoLoader loader;
        if( loader.LoadFromFile( filename ) == TS_FALSE )
            return TS_FALSE;

        for( TsInt i = 0; i < loader.GetMeshNum(); ++i )
        {
            TsVertexBuffer* buffer = pDev->CreateVertexBuffer(
                loader.GetVertexBuffer( i ) ,
                loader.GetVertexSize( i ) ,
                loader.GetVertexStride() );

            TsMesh * mesh = TsNew( TsMesh );
            mesh->m_vertexBuffer = buffer;

            TsMaterial* material = TsNew( TsMaterial);
            material->m_textureName = loader.GetTexturePass( i );
            material->LoadTextureFromFile( pDev );

            TsGeometryObject *  obj = TsNew( TsGeometryObject );
            obj->CreateGeometryObject( pDev , mesh , material );
            m_pObjects.push_back( obj );
        }

        return TS_TRUE;
    }

    if( filename.rfind( ".fbx" ) != TsString::npos || 
        filename.rfind( ".FBX" ) != TsString::npos)
    {
        TsFbxLoader loader;
        if( loader.LoadFromFile( filename ) == TS_FALSE )
            return TS_FALSE;

        for( TsInt i = 0; i < loader.GetMeshNum(); ++i )
        {
            TsVertexBuffer* buffer = pDev->CreateVertexBuffer(
                loader.GetVertexBuffer( i ) ,
                loader.GetVertexSize( i ) ,
                loader.GetVertexStride() );

            TsIndexBuffer* indexBuffer = pDev->CreateIndexBuffer(
                                        loader.GetIndexBuffer(i),
                                        loader.GetIndexBufferSize(i));

            TsMesh * mesh = TsNew( TsMesh );
            mesh->m_vertexBuffer = buffer;
            mesh->m_indexBuffer = indexBuffer;
            TsLambertMatrial* material = TsNew( TsLambertMatrial );

            TSUT::TsFilePathAnalyzer ana = filename;
            material->m_textureName = ana.GetLocalDirectory() + loader.GetAlbedoTexturePass( i );

            material->LoadTextureFromFile( pDev );
            TsInt id = mesh->GetIndexNum();
            TsGeometryObject * obj = nullptr;
            if( loader.IsSkinMesh( i ) )
            {
                TsSkinGeometryObject * skin =
                    TsNew( TsSkinGeometryObject );
                skin->CreateGeometryObject( pDev , mesh , material );
                skin->SetSkeleton( loader.GetSkeleton() );
                obj = skin;
                
            }
            else
            {
                obj = TsNew( TsGeometryObject );
                obj->CreateGeometryObject( pDev , mesh , material );
            }
  
            TsTransForm* pTransform = loader.GetTransform(i);
            
            obj->SetName( pTransform->GetName() );
            obj->SetTransform( pTransform );
            m_pObjects.push_back( obj );
        }

        return TS_TRUE;
    }
    return TS_FALSE;
}

TsMaterial* TsMeshFactory::GetMaterialArray()
{
    return nullptr;
}

TsGeometryObject* TsMeshFactory::GetGeometryObject( TsInt index  )
{
    return m_pObjects[index];
}