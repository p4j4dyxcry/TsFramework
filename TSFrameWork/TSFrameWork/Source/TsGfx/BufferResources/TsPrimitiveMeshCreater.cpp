#include "../TsGfx.h"

template <typename T>
TsBool GenerateSphereTemplate( T** pOut,
                               TsInt& outCount ,
                               TsInt div)
{
    T * pVertex;
    TsSphereMeshCreater creater;

    creater.CreateSphere(div);
    auto posList = creater.GetPositions();
    auto nomList = creater.GetNomals();
    auto index = creater.GetIndex();

    outCount = index.size();
    if (index.empty())
        return nullptr;

    pVertex = TsNew(T[outCount]);
    memset(pVertex, 0, sizeof(T)* outCount);
    for (TsInt i = 0; i < outCount; ++i)
    {
        pVertex[i].pos = posList[index[i]];
        pVertex[i].normal = nomList[index[i]];
    }

    *pOut = pVertex;

    return TS_TRUE;
}

template <typename T>
TsBool GenerateSpriteTemplate( T** pOut ,
                               TsInt& outCount ,
                               TS_CULL_MODE cullmode)
{
    T * pVertex = nullptr;

    if (cullmode == TS_CULL_MODE::NONE)
        outCount = 12;
    else
        outCount = 6;

    pVertex = TsNew(T[outCount]);

    memset(pVertex, 0, sizeof(T)* outCount);

    TsVector3 left_up = TsVector3(-0.5, 0.5, 0);
    TsVector3 left_down = TsVector3(-0.5, -0.5, 0);
    TsVector3 right_up = TsVector3(0.5, 0.5, 0);
    TsVector3 right_down = TsVector3(0.5, -0.5, 0);

    TsInt index = 0;

    //ëOÇÃñ 
    if (cullmode != TS_CULL_MODE::FRONT)
    {
        pVertex[index++].pos = left_up;
        pVertex[index++].pos = right_up;
        pVertex[index++].pos = left_down;

        pVertex[index++].pos = right_up;
        pVertex[index++].pos = left_down;
        pVertex[index++].pos = right_down;
    }

    //å„ÇÎÇÃñ 
    if (cullmode != TS_CULL_MODE::BACK)
    {
        pVertex[index++].pos = right_up;
        pVertex[index++].pos = left_up;
        pVertex[index++].pos = right_down;

        pVertex[index].pos = left_up;
        pVertex[index].pos = right_down;
        pVertex[index].pos = left_down;
    }

    *pOut = pVertex;

    return TS_TRUE;
}

template <typename T>
TsBool GenerateBox3DTemplate( T** pOut,
                              TsInt& outCount)
{
    T * pVertex = nullptr;

    outCount = 36;

    pVertex = TsNew(T[outCount]);

    memset(pVertex, 0, sizeof(T)* outCount);


    TsVector3 left_up_back = TsVector3(-0.5, 0.5, 0.5);
    TsVector3 left_down_back = TsVector3(-0.5, -0.5, 0.5);
    TsVector3 left_up_front = TsVector3(-0.5, 0.5, -0.5);
    TsVector3 left_down_front = TsVector3(-0.5, -0.5, -0.5);
    TsVector3 right_up_back = TsVector3(0.5, 0.5, 0.5);
    TsVector3 right_down_back = TsVector3(0.5, -0.5, 0.5);
    TsVector3 right_up_front = TsVector3(0.5, 0.5, -0.5);
    TsVector3 right_down_front = TsVector3(0.5, -0.5, -0.5);

    //ñ@ê¸Çê›íËÇ∑ÇÈ
    for (TsInt i = 0; i < 6; ++i)
        pVertex[i].normal = TsVector3::up;
    for (TsInt i = 0; i < 6; ++i)
        pVertex[6 + i].normal = TsVector3::down;
    for (TsInt i = 0; i < 6; ++i)
        pVertex[12 + i].normal = TsVector3::left;
    for (TsInt i = 0; i < 6; ++i)
        pVertex[18 + i].normal = TsVector3::right;
    for (TsInt i = 0; i < 6; ++i)
        pVertex[24 + i].normal = TsVector3::front;
    for (TsInt i = 0; i < 6; ++i)
        pVertex[30 + i].normal = TsVector3::back;

    //è„ÇÃñ 
    pVertex[0].pos = left_up_back;
    pVertex[1].pos = right_up_back;
    pVertex[2].pos = left_up_front;

    pVertex[3].pos = right_up_back;
    pVertex[4].pos = right_up_front;
    pVertex[5].pos = left_up_front;

    //â∫ÇÃñ 
    pVertex[6].pos = right_down_back;
    pVertex[7].pos = left_down_back;
    pVertex[8].pos = left_down_front;

    pVertex[9].pos = right_down_back;
    pVertex[10].pos = left_down_front;
    pVertex[11].pos = right_down_front;

    //ç∂ÇÃñ 
    pVertex[12].pos = left_up_back;
    pVertex[13].pos = left_up_front;
    pVertex[14].pos = left_down_back;

    pVertex[15].pos = left_up_front;
    pVertex[16].pos = left_down_front;
    pVertex[17].pos = left_down_back;

    //âEÇÃñ 
    pVertex[18].pos = right_up_front;
    pVertex[19].pos = right_up_back;
    pVertex[20].pos = right_down_front;

    pVertex[21].pos = right_up_back;
    pVertex[22].pos = right_down_back;
    pVertex[23].pos = right_down_front;

    //ëOÇÃñ 
    pVertex[24].pos = left_up_front;
    pVertex[25].pos = right_up_front;
    pVertex[26].pos = left_down_front;

    pVertex[27].pos = right_up_front;
    pVertex[28].pos = right_down_front;
    pVertex[29].pos = left_down_front;
    //å„ÇÎÇÃñ 
    pVertex[30].pos = right_up_back;
    pVertex[31].pos = left_up_back;
    pVertex[32].pos = right_down_back;

    pVertex[33].pos = left_up_back;
    pVertex[34].pos = left_down_back;
    pVertex[35].pos = right_down_back;

    *pOut = pVertex;

    return TS_TRUE;
}

template <typename T>
TsBool GenerateSpriteLineTemplate( T** pOut ,
                               TsInt& outCount)
{
    T * pVertex = nullptr;

    outCount = 8;

    pVertex = TsNew( T[outCount] );

    memset( pVertex , 0 , sizeof( T )* outCount );

    TsVector3 left_up = TsVector3( -0.5 , 0.5 , 0 );
    TsVector3 left_down = TsVector3( -0.5 , -0.5 , 0 );
    TsVector3 right_up = TsVector3( 0.5 , 0.5 , 0 );
    TsVector3 right_down = TsVector3( 0.5 , -0.5 , 0 );

    TsInt index = 0;


    pVertex[index++].pos = left_up;
    pVertex[index++].pos = left_down;

    pVertex[index++].pos = left_up;
    pVertex[index++].pos = right_up;

    pVertex[index++].pos = right_up;
    pVertex[index++].pos = right_down;

    pVertex[index++].pos = right_down;
    pVertex[index++].pos = left_down;


    *pOut = pVertex;

    return TS_TRUE;
}

template <typename T>
TsBool GenerateBox3DLineTemplate( T** pOut ,
                              TsInt& outCount )
{
    T * pVertex = nullptr;

    outCount = 24;

    pVertex = TsNew( T[outCount] );

    memset( pVertex , 0 , sizeof( T )* outCount );


    TsVector3 left_up_back = TsVector3( -0.5 , 0.5 , 0.5 );
    TsVector3 left_down_back = TsVector3( -0.5 , -0.5 , 0.5 );
    TsVector3 left_up_front = TsVector3( -0.5 , 0.5 , -0.5 );
    TsVector3 left_down_front = TsVector3( -0.5 , -0.5 , -0.5 );
    TsVector3 right_up_back = TsVector3( 0.5 , 0.5 , 0.5 );
    TsVector3 right_down_back = TsVector3( 0.5 , -0.5 , 0.5 );
    TsVector3 right_up_front = TsVector3( 0.5 , 0.5 , -0.5 );
    TsVector3 right_down_front = TsVector3( 0.5 , -0.5 , -0.5 );

    TsInt i = 0;
    //ç∂ë§ÇÃÇSï”
    {
        pVertex[i++].pos = left_up_back;
        pVertex[i++].pos = left_down_back;

        pVertex[i++].pos = left_up_front;
        pVertex[i++].pos = left_down_front;

        pVertex[i++].pos = left_up_front;
        pVertex[i++].pos = left_up_back;

        pVertex[i++].pos = left_down_front;
        pVertex[i++].pos = left_down_back;
    }
    //âEë§ÇÃÇSï”
    {
        pVertex[i++].pos = right_up_back;
        pVertex[i++].pos = right_down_back;

        pVertex[i++].pos = right_up_front;
        pVertex[i++].pos = right_down_front;

        pVertex[i++].pos = right_up_front;
        pVertex[i++].pos = right_up_back;

        pVertex[i++].pos = right_down_front;
        pVertex[i++].pos = right_down_back;
    }
    //íÜâõÇÃÇSï”
    {
        pVertex[i++].pos = left_up_back;
        pVertex[i++].pos = right_up_back;

        pVertex[i++].pos = left_down_back;
        pVertex[i++].pos = right_down_back;

        pVertex[i++].pos = left_up_front;
        pVertex[i++].pos = right_up_front;

        pVertex[i++].pos = left_down_front;
        pVertex[i++].pos = right_down_front;
    }

    *pOut = pVertex;

    return TS_TRUE;
}

TsBool TsPrimitiveMeshCreater::GenerateSphere(TsVertexSkin** pOutVertex,
                                              TsInt& outVertexCount,
                                              TsInt div)
{
    return GenerateSphereTemplate(pOutVertex, outVertexCount, div);
}
TsBool TsPrimitiveMeshCreater::GenerateSphere(TsVertexDefault** pOutVertex,
                                               TsInt& outVertexCount,
                                               TsInt div )
{
    return GenerateSphereTemplate(pOutVertex, outVertexCount, div);
}

TsBool TsPrimitiveMeshCreater::GenerateSprite(TsVertexSkin** pOutVertex,
                                              TsInt& outVertexCount,
                                              TS_CULL_MODE cullMode)
{
    return GenerateSpriteTemplate(pOutVertex, outVertexCount, cullMode);
}
TsBool TsPrimitiveMeshCreater::GenerateSprite(TsVertexDefault** pOutVertex,
                                              TsInt& outVertexCount,
                                              TS_CULL_MODE cullMode)
{
    return GenerateSpriteTemplate(pOutVertex, outVertexCount, cullMode);
}

TsBool TsPrimitiveMeshCreater::GenerateBox3D(TsVertexSkin** pOutVertex,
                     TsInt& outVertexCount)
{
    return GenerateBox3DTemplate(pOutVertex, outVertexCount);
}
TsBool TsPrimitiveMeshCreater::GenerateBox3D( TsVertexDefault** pOutVertex ,
                       TsInt& outVertexCount)
{
    return GenerateBox3DTemplate(pOutVertex, outVertexCount);
}

TsBool TsPrimitiveMeshCreater::GenrateInvBox3D( TsVertexSkin** pOutVertex ,
                       TsInt& outVertexCount)
{
    return TS_FALSE;
}
TsBool TsPrimitiveMeshCreater::GenrateInvBox3D( TsVertexDefault** pOutVertex ,
                        TsInt& outVertexCount)
{
    return TS_FALSE;
}
TsBool TsPrimitiveMeshCreater::GenrateInvBox3D( TSVertexSkyBox** pOutVertex ,
                       TsInt& outVertexCount)
{
    return TS_FALSE;
}

TsBool TsPrimitiveMeshCreater::GenerateSpriteLine( TsVertexSkin** ppOutVertex ,
                                                   TsInt& outVertexCount )
{
    return GenerateSpriteLineTemplate( ppOutVertex , outVertexCount );
}
TsBool TsPrimitiveMeshCreater::GenerateSpriteLine( TsVertexDefault** ppOutVertex ,
                                                   TsInt& outVertexCount )
{
    return GenerateSpriteLineTemplate( ppOutVertex , outVertexCount );
}

TsBool TsPrimitiveMeshCreater::GenerateBox3DLine( TsVertexSkin** ppOutVertex ,
                             TsInt& outVertexCount )
{
    return GenerateBox3DLineTemplate( ppOutVertex , outVertexCount );
}
TsBool TsPrimitiveMeshCreater::GenerateBox3DLine( TsVertexDefault** ppOutVertex ,
                             TsInt& outVertexCount )
{
    return GenerateBox3DLineTemplate( ppOutVertex , outVertexCount );
}