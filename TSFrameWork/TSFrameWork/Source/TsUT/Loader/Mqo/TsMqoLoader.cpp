#include "../../TsUT.h"
#include "../../../TsMath/TsMath.h"
#include <fstream>
#include <sstream>

TsMqoLoader::TsMqoLoader() :
m_mqoObject( nullptr ),
m_loadScale(1),
m_isSmooth(TS_TRUE)
{

};

TsMqoLoader::~TsMqoLoader()
{
    TsSafeDelete( m_mqoObject );
}

TsBool TsMqoLoader::LoadFile( const TsChar* filename )
{
    Ts3DModelLoader::LoadFile( filename );
    std::ifstream ifs( filename );

    if( ifs.fail() )
    {
        TsDebugLogError("Load Mqo Fail \n\t file %s \n", filename);
        return TS_FALSE;
    }
    
    TsString str;
    TsVector<TsFloat3> posList;
    TsVector<TsMqoMesh> meshList;
    TsVector<TsMqoMaterial> materialList;
    TsVector<TsVector<TsMqoFace>> faceList;
    
    while( !ifs.eof() )
    {
        std::getline( ifs , str );

        if( str.find( "Material" ) == 0 )
        {
            materialList = LoadMaterial( ifs );
        }
        else if( str.find( "Object" ) == 0 )
        {
            faceList.push_back( LoadObject( posList, ifs ) );
        }
    }

    //マテリアルが含まれていない場合はマテリアルを作成する
    if( materialList.size() == 0 )
    {
        materialList.push_back( TsMqoMaterial() );
        materialList[0].color = TsFloat4( 1 , 1 , 1 , 1 );
        materialList[0].diffuse = 1;
        materialList[0].emissib = 0;
        materialList[0].ambient = 0;
        materialList[0].specular = 0;
        materialList[0].specularPower = 0;
    }

    //重複頂点の列挙
    TsVector<TsVector<TsMqoFace>> posRefList;
    posRefList.resize( posList.size() );
    for( auto fl : faceList )
    {
        for( auto tl : fl )
        {
            for( TsInt i = 0; i < 3; ++i )
            {
                TsInt idx = tl.index[i];
                posRefList[idx].push_back( tl );
            }
        }
    }

    for( TsInt i = 0; i < (TsInt)materialList.size();++i )
    {
        TsMqoMesh mesh;
        mesh.material = materialList[i];
        mesh.vertex.reserve( 65535 );
        meshList.push_back( mesh );
    }

    //頂点リストの作成
    for( auto fl : faceList )
    {
        for( auto tl : fl )
        {
            for( TsInt i = 0; i < 3; ++i )
            {
                //法線のスムース
                TsFloat3 normal(0,0,0);

                if( m_isSmooth )
                {
                    for( auto pr : posRefList[tl.index[i]] )
                    {
                        if( TsFloat3::Dot( tl.normal , pr.normal ) < TsRadian( 45.0f ) )
                        {
                            TsFloat3 n = pr.normal;
                            n *= ( pr.angle * pr.area ) / tl.angle / pr.area;
                            normal += n;
                        }
                    }
                }
                else
                    normal = tl.normal;

                normal.Normalize();
                TsMqoVertex v;
                v.uv = tl.uv[i];
                v.normal = normal;
                v.pos = posList[tl.index[i]]*m_loadScale;

                TsUint matIdx = tl.m_matIndex;
                meshList[tl.m_matIndex].vertex.push_back( v );
            }
        }
    }

    m_mqoObject = TsNew( TsMqoObject );
    m_mqoObject->mesh = meshList;
    return TS_TRUE;
}

TsBool TsMqoLoader::CreateCommonData()
{
    m_meshCount = m_mqoObject->mesh.size();
    m_materialCount = m_mqoObject->mesh.size();

    m_pMaterials = TsNew( TsCommon3DMaterial[m_materialCount] );
    m_pMeshs = TsNew( TsCommon3DMesh[m_meshCount] );

    auto  TsFloat2ToTsVector2 = []( TsFloat2 f )
    {
        return TsVector2( f.x , f.y );
    };

    auto  TsFloat3ToTsVector3 =[]( TsFloat3 f )
    {
        return TsVector3( f.x , f.y , f.z );
    };

    for( TsUint i = 0; i < m_materialCount; ++i )
    {
        TsCommon3DMaterial& mat = m_pMaterials[i];
        TsMqoMaterial mqoMat = m_mqoObject->mesh[i].material;
        mat.m_name = GetName() + "_Material" + TSUT::IntToString( i );
        mat.m_diffuse = TsFloat3( mqoMat.color.x , mqoMat.color.y , mqoMat.color.z) * mqoMat.diffuse;
        mat.m_ambient = TsFloat3( mqoMat.ambient , mqoMat.ambient , mqoMat.ambient );
        mat.m_alpha = mqoMat.color.w;
        mat.m_specluer = TsFloat3( mqoMat.specular , mqoMat.specular , mqoMat.specular );
        mat.m_power = mqoMat.specularPower;
        mat.m_albedoTexture = mqoMat.textureName;
    }

    for( TsUint i = 0; i < m_meshCount; ++i )
    {
        TsCommon3DMesh& mesh = m_pMeshs[i];
        TsMqoMesh             mqoMesh = m_mqoObject->mesh[i];
        mesh.m_vertexCount = mqoMesh.vertex.size();
        mesh.m_pPositions = TsNew( TsVector3[mesh.m_vertexCount] );
        mesh.m_pNormals = TsNew( TsVector3[mesh.m_vertexCount] );
        mesh.m_pTexcoords = TsNew( TsVector2[mesh.m_vertexCount]);

        mesh.m_name = GetName() + "_MqoMesh" + TSUT::IntToString( i );

        for( TsUint j = 0; j < mesh.m_vertexCount; ++j )
        {
            mesh.m_pPositions[j] = TsFloat3ToTsVector3( mqoMesh.vertex[j].pos);
            mesh.m_pNormals[j] = TsFloat3ToTsVector3( mqoMesh.vertex[j].normal);
            mesh.m_pTexcoords[j] = TsFloat2ToTsVector2( mqoMesh.vertex[j].uv);
            mesh.m_aabb.AddPoint( mesh.m_pPositions[j] );
        }
        mesh.m_pMaterialRef = &m_pMaterials[i];
        mesh.m_pTransoform = TsNew( TsTransForm );
    }
    return TS_TRUE;
}

TsVector<TsMqoFace> TsMqoLoader::LoadObject(TsVector<TsFloat3>& position, 
                                             std::ifstream & ifs )
{
    TsVector<TsMqoFace> result;
    result.reserve( 4096 );
    position.reserve( 4096 * 3 );
    TsString str;
    while( !ifs.eof() )
    {
        std::getline( ifs , str );
        if( str.find( "}" ) == 0 ) break;

        if( str.find( "visible " ) != TsString::npos ) {
            std::stringstream ss( str );
            TsString s;
            TsInt visible;
            ss >> s >> visible;
            if( !visible ) break;
        }

        if( str.find( "vertex " ) != TsString::npos )
        {
            while( !ifs.eof() )
            {
                getline( ifs , str );
                if( str.find( "}" ) != TsString::npos )
                {
                    break;	// "}"があると終了
                }
                std::stringstream ss( str );
                TsFloat3 v;
                ss >> v.x >> v.y >> v.z;
                position.push_back( v );
            }
        }

        if( str.find( "face " ) != TsString::npos )
        {
            while( !ifs.eof() )
            {
                getline( ifs , str );

                //終了
                if( str.find( "}" ) != TsString::npos )
                    break;

                if( str[str.find_first_not_of( " \t" )] == '3' )
                {
                    TsMqoFace face;
                    std::stringstream ss( str.substr( str.find( "V(" ) + 2 ) );
                    ss  >> face.index[2]
                        >> face.index[1]
                        >> face.index[0];
                    if( str.find( "M(" ) != TsString::npos )
                    {
                        ss.str( str.substr( str.find( "M(" ) + 2 ) );
                        ss >> face.m_matIndex;
                    }
                    else
                    {
                        face.m_matIndex = 0;
                    }
                    //UV情報の格納
                    ss.str( str.substr( str.find( "UV(" ) + 3 ) );

                    ss  >> face.uv[2].x >> face.uv[2].y
                        >> face.uv[1].x >> face.uv[1].y
                        >> face.uv[0].x >> face.uv[0].y;

                    // 辺を算出
                    TsFloat3 v0 = position[face.index[0]] - position[face.index[1]];
                    TsFloat3 v1 = position[face.index[1]] - position[face.index[2]];

                    //! Compute Normal
                    TsFloat3 cross = TsFloat3::Cross(v0,v1);
                    face.normal = cross;
                    face.normal.Normalize();

                    //! Compute Angle
                    v0.Normalize();
                    v1.Normalize();
                    face.angle = acos ( TsFloat3::Dot(v0,v1) );
                    
                    //! Compute Area
                    face.area = cross.Length() / 2.0f;

                    result.push_back( face );
                }
                if( str[str.find_first_not_of( " \t" )] == '4' )
                {
                    TsMqoFace face[2];
                    std::stringstream ss( str.substr( str.find( "V(" ) + 2 ) );
                    TsInt4 idx;
                    ss >> idx[3] >> idx[2] >> idx[1] >> idx[0];

                    for (TsInt i = 0; i < 3; ++i)
                        face[0].index[i] = idx[i];

                    face[1].index[0] = idx[3];
                    face[1].index[1] = idx[0];
                    face[1].index[2] = idx[2];

                    //マテリアル情報の格納
                    if( str.find( "M(" ) != TsString::npos )
                    {
                        ss.str( str.substr( str.find( "M(" ) + 2 ) );
                        ss >> face[0].m_matIndex;
                        face[1].m_matIndex = face[0].m_matIndex;
                    }
                    else
                    {
                        face[0].m_matIndex = face[1].m_matIndex = 0;
                    }

                    //UV情報の格納
                    ss.str( str.substr( str.find( "UV(" ) + 3 ) );
                    TsFloat2 uv[4];

                    ss  >> uv[3].x >> uv[3].y
                        >> uv[2].x >> uv[2].y
                        >> uv[1].x >> uv[1].y
                        >> uv[0].x >> uv[0].y;

                    for (TsInt i = 0; i < 3; ++i)
                        face[0].uv[i] = uv[i];
                    face[1].uv[0] = uv[3];
                    face[1].uv[1] = uv[0];
                    face[1].uv[2] = uv[2];

                    for( TsInt i = 0; i < 2; ++i )
                    {
                        // 辺を算出
                        TsFloat3 v0 = position[face[i].index[0]] - position[face[i].index[1]];
                        TsFloat3 v1 = position[face[i].index[1]] - position[face[i].index[2]];

                        //! Compute Normal
                        TsFloat3 cross = TsFloat3::Cross( v0 , v1 );
                        face[i].normal = cross;
                        face[i].normal.Normalize();

                        //! Compute Angle
                        v0.Normalize();
                        v1.Normalize();
                        face[i].angle = acos( TsFloat3::Dot( v0 , v1 ) );

                        //! Compute Area
                        face[i].area = cross.Length() / 2.0f;

                        result.push_back( face[i] );
                    }
                }
            }
        }
    }

    return result;
}

//! マテリアル構文解析
TsVector<TsMqoMaterial> TsMqoLoader::LoadMaterial( std::ifstream & ifs )
{
    TsVector<TsMqoMaterial> result;
    TsString str;
    while( !ifs.eof() )
    {
        std::getline( ifs , str );
        if( str.find( "}" ) == 0 ) break;	// 行の先頭に"}"があると終了
        TsVector<TsF32> col = PickOutMaterial<TsF32>( str , "col" , 4 );		//カラーを4つ取得
        TsVector<TsF32> dif = PickOutMaterial<TsF32>( str , "dif" , 1 );		//ディフューズを「1つ」取得（メタセコイアには一つしかない。
        TsVector<TsF32> amb = PickOutMaterial<TsF32>( str , "amb" , 1 );		//アンビエントの取得
        TsVector<TsF32> emi = PickOutMaterial<TsF32>( str , "emi" , 1 );		//エミッシブル（自己発光）の取得	
        TsVector<TsF32> spc = PickOutMaterial<TsF32>( str , "spc" , 1 );		//スペキュラの取得
        TsVector<TsF32> power = PickOutMaterial<TsF32>( str , "power" , 1 );	//スぺキャラパワーの取得
        TsVector<TsString> tex = PickOutMaterial<TsString>( str , "tex" , 1 );	//テクスチャの取得

        TsMqoMaterial material;
        material.color.x = col[0];
        material.color.y = col[1];
        material.color.z = col[2];
        material.color.w = col[3];

        material.diffuse = dif[0];
        material.ambient = amb[0];
        material.emissib = emi[0];
        material.specular = spc[0];
        material.specularPower = power[0];
        //if( tex.size() != 0 )
        //	material.textureName = tex[0].substr(1,tex[0].size()-2);

        result.push_back( material );
    }

    return result;
}
template <typename T> TsVector<T> TsMqoLoader::PickOutMaterial(TsString str, TsString name, TsInt num)
{
    TsVector<T> ts;
    T t;
    TsInt offset = str.find("(", str.find(name, str.find("\"", str.find("\""))));// "材質名"の後の、nameの後の、括弧の位置

    if( offset == TsString::npos )
        return ts;
    else 
        ++offset;								// 括弧の次の位置

    TsInt count = str.find( ")" , offset ) - offset;	// offsetから閉じ括弧までの長さ
    TsString s = str.substr( offset , count );		// 指定した名前のデータを抽出
    std::stringstream ss( s );
    for (TsInt i = 0; i < num; ++i)
    {
        ss >> t;
        ts.push_back( t );
    }
    return ts;
}

void TsMqoLoader::SetCenti( TsBool isCenti )
{
    if( isCenti )
        m_loadScale = 1;	//centi
    else
        m_loadScale = 0.01f;//matle
}

void TsMqoLoader::SetSmooth( TsBool isSmooth )
{
    m_isSmooth = isSmooth;
}
