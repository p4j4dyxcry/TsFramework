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
        return TS_FALSE;

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

    //前の面
    if (cullmode != TS_CULL_MODE::FRONT)
    {
        pVertex[index++].pos = left_up;
        pVertex[index++].pos = right_up;
        pVertex[index++].pos = left_down;

        pVertex[index++].pos = right_up;
        pVertex[index++].pos = left_down;
        pVertex[index++].pos = right_down;
    }

    //後ろの面
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

    //法線を設定する
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

    //上の面
    pVertex[0].pos = left_up_back;
    pVertex[1].pos = right_up_back;
    pVertex[2].pos = left_up_front;

    pVertex[3].pos = right_up_back;
    pVertex[4].pos = right_up_front;
    pVertex[5].pos = left_up_front;

    //下の面
    pVertex[6].pos = right_down_back;
    pVertex[7].pos = left_down_back;
    pVertex[8].pos = left_down_front;

    pVertex[9].pos = right_down_back;
    pVertex[10].pos = left_down_front;
    pVertex[11].pos = right_down_front;

    //左の面
    pVertex[12].pos = left_up_back;
    pVertex[13].pos = left_up_front;
    pVertex[14].pos = left_down_back;

    pVertex[15].pos = left_up_front;
    pVertex[16].pos = left_down_front;
    pVertex[17].pos = left_down_back;

    //右の面
    pVertex[18].pos = right_up_front;
    pVertex[19].pos = right_up_back;
    pVertex[20].pos = right_down_front;

    pVertex[21].pos = right_up_back;
    pVertex[22].pos = right_down_back;
    pVertex[23].pos = right_down_front;

    //前の面
    pVertex[24].pos = left_up_front;
    pVertex[25].pos = right_up_front;
    pVertex[26].pos = left_down_front;

    pVertex[27].pos = right_up_front;
    pVertex[28].pos = right_down_front;
    pVertex[29].pos = left_down_front;
    //後ろの面
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
    //左側の４辺
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
    //右側の４辺
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
    //中央の４辺
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

TsBool TsPrimitiveMeshCreater::GenerateCapsule3D(TsVertexSkin** ppOutVertex,
                                                 TsInt& outVertexCount,
                                                 TsUint slices, TsUint stack)
{
    TsF32     inv_stack = 1.0f / stack;
    TsF32     inv_slice = 1.0f / slices;

    TsInt vertex_count = ( slices * stack + 1 ) * 2;

    TsVector<TsVertexSkin> original_vertices(vertex_count);
    TsVertexSkin* ptr = &original_vertices[0];

    auto SetVertex = [](TsVertexSkin*& ptr, TsVector3 point, TsVector3 normal)
    {
        ptr->pos = point + normal;
        ptr->normal = normal;
        ++ptr;
    };

    //-----------------------------------------------------------------------
    // 球を2つ作成
    //-----------------------------------------------------------------------

    //! 上側の半球
    SetVertex(ptr, TsVector3::up, TsVector3::up);
    for (TsUint i = 0; i < stack; ++i)
    {
        TsF32 t = ( TS_PI * 0.5f) * inv_stack * ( i + 1);
        TsF32 r = sinf(t);
        TsVector3 x = TsVector3::right * r;
        TsVector3 y = TsVector3::front * r;
        TsVector3 z = TsVector3::up * cosf(t);

        for (TsUint j = 0; j < slices; ++j)
        {
            TsF32 u = TS_PI * 2.0f * inv_slice * j;
            TsVector3 n = x * cosf(u) + y * sinf(u) + z;

            SetVertex(ptr, TsVector3::up, n);
        }
    }

    //! 下側の半球
    for (TsUint i = 0; i < stack; ++i)
    {
        TsF32 t = TS_PI * inv_stack * i;
        TsF32 r = cosf(t);
        TsVector3 x = TsVector3::right * r;
        TsVector3 y = TsVector3::front * r;
        TsVector3 z = TsVector3::up * -sinf(t);

        for (TsUint j = 0; j < slices; ++j)
        {
            TsF32 u = TS_PI * 2.0f * inv_slice * j;
            TsVector3 n = x * cosf(u) + y * sinf(u) + z;
            SetVertex(ptr, TsVector3::down, n);
        }
    }
    SetVertex(ptr, TsVector3::down, TsVector3::down);

    //------------------------------------------------------------------
    // 2つの半球をつなげる
    //------------------------------------------------------------------

    outVertexCount = slices*stack * 2 * 2 * 3;
    *ppOutVertex = TsNew(TsVertexSkin[outVertexCount]);
    TsVertexSkin* pOut = ppOutVertex[0];

    // 上半球側端点周りの計算
    for (TsUint i = 0; i<slices; i++)
    {
        *(pOut++) = original_vertices[0];
        *(pOut++) = original_vertices[1 + i];
        *(pOut++) = original_vertices[1 + (i+1)%slices];
    }

    // 上半球と下半球をつなげる
    TsUint imax = stack * 2 - 1;
    for (TsUint i = 0; i<imax; i++)
    {
        TsUint ibase = 1 + slices*i;
        for (TsUint j = 0; j<slices; j++)
        {
            TsUint jnext = (j + 1) % slices;

            *(pOut++) = original_vertices[ibase + j];
            *(pOut++) = original_vertices[ibase + j + slices];
            *(pOut++) = original_vertices[ibase + jnext];

            *(pOut++) = original_vertices[ibase + jnext];
            *(pOut++) = original_vertices[ibase + j + slices];
            *(pOut++) = original_vertices[ibase + jnext + slices];
        }
    }

    // 下半球の側端点周りの計算
    TsUint ibase = 1 + slices*imax;
    TsUint ilast = vertex_count - 1;
    for (TsUint j = 0; j<slices; j++)
    {
        *(pOut++) = original_vertices[ibase + j];
        *(pOut++) = original_vertices[ilast];
        *(pOut++) = original_vertices[ibase + (j + 1) % slices];
    }
    return TS_TRUE;
}