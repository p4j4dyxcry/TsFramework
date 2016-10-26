#include "../../../TsAfx.h"

TsMeshFactory::TsMeshFactory()
{

}
TsMeshFactory::~TsMeshFactory()
{

}

TsTransformBakeAnimation* TsMeshFactory::CreateBakeAnimation( const TsString& filename )
{
    TsMeshLoader::TsLoadOption opt;
    opt.loadAnimation = TS_TRUE;
    opt.loadGeomery = TS_FALSE;

    TsFbxLoader loader;
    loader.LoadFromFile( filename ,opt);
    return loader.CreateAnimation(0);
}

TsBool TsMeshFactory::LoadModelFromFile(TsDevice* pDev, 
                                        TsString filename,
                                        TsString name)
{
   TsMeshObject* pMesh = TsNew(TsMeshObject);
   pMesh->SetName(name);
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

            TsVertexElement * mesh = TsNew( TsVertexElement );
            mesh->m_vertexBuffer = buffer;

            TsMaterial* material = TsNew( TsMaterial);
            material->m_textureName = loader.GetTexturePass( i );
            material->LoadTextureFromFile( pDev );

            TsGeometryObject *  obj = TsNew( TsGeometryObject );
            obj->CreateGeometryObject( pDev , mesh , material );
            pMesh->AddGeometry(obj);
        }
        TsResourceManager::RegisterResource<TsMeshObject>(pMesh, name);
        return TS_TRUE;
    }

    if( filename.rfind( ".fbx" ) != TsString::npos || 
        filename.rfind( ".FBX" ) != TsString::npos)
    {
        TsMeshLoader::TsLoadOption opt;
        opt.loadAnimation = TS_FALSE;

        TsFbxLoader loader;
        if( loader.LoadFromFile( filename ,opt) == TS_FALSE )
            return TS_FALSE;
        TsSkeleton* pSkeleton = loader.GetSkeleton();
        if( pSkeleton )
            TsResourceManager::RegisterResource<TsSkeleton>( pSkeleton ,pSkeleton->GetName());
        for( TsInt i = 0; i < loader.GetMeshNum(); ++i )
        {
            TsVertexBuffer* buffer = pDev->CreateVertexBuffer(
                loader.GetVertexBuffer( i ) ,
                loader.GetVertexSize( i ) ,
                loader.GetVertexStride() );

            TsIndexBuffer* indexBuffer = pDev->CreateIndexBuffer(
                                        loader.GetIndexBuffer(i),
                                        loader.GetIndexBufferSize(i));

            TsVertexElement * mesh = TsNew( TsVertexElement );
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
                skin->SetSkeleton( pSkeleton );
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
            pMesh->AddGeometry(obj);
        }
        TsResourceManager::RegisterResource<TsMeshObject>(pMesh, name);
        return TS_TRUE;
    }
    TsDelete(pMesh);
    return TS_FALSE;
}