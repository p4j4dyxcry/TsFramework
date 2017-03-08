#include "../../../../TsAfx.h"
#include <fstream>
#include <sstream>
#include <algorithm>

//----------------------------------------------------------
//! コンストラクタ・デストラクタ
//----------------------------------------------------------
TsOBJLoader::TsOBJLoader(){}
TsOBJLoader::~TsOBJLoader(){}

//----------------------------------------------------------
//! ファイル読み込み
//----------------------------------------------------------
TsBool TsOBJLoader::LoadFile( const TsChar* filename )
{
    Ts3DModelLoader::LoadFile( filename );
    return LoadObj(filename);
}

//----------------------------------------------------------
//! マテリアル(*.mtl)を出力する
//----------------------------------------------------------
TsBool TsOBJLoader::SaveMaterial(const TsChar* filename)
{
    std::ofstream ofs(filename);

    //! ストリームに float データを単体出力
    auto WriteFloat1 = [](std::ofstream& ofs, const TsChar* head, TsF32 param)
    {
        ofs << head << " " << param << std::endl;
    };

    //! ストリームに Vector3 データを出力
    auto WriteFloat3 = [](std::ofstream& ofs, const TsChar* head, TsFloat3& param)
    {
        ofs << head << " " << param.x << 
                       " " << param.y << 
                       " " << param.z << std::endl;
    };

    //! ストリームに String データを出力
    auto WritePath = [](std::ofstream& ofs, const TsChar* head, TsString& param)
    {
        if (param.size() > 0)
            ofs << head << " " << param << std::endl;
    };

    //! materialの使用パラメータからデータを出力する
    for (TsUint i = 0; i < m_materialList.size(); ++i)
    {
        TsObjMaterial& m = m_materialList[i];
        TsObjMaterialUse& op= m.usingMaterialParam;

        ofs << "newmtl" << " " << m.name << std::endl;

        if (op.useDiffuse)
            WriteFloat3(ofs, "Kd", m.diffuse);

        if (op.useAmbient)
            WriteFloat3(ofs, "Ka", m.ambient);

        if (op.useSpecluer)
            WriteFloat3(ofs, "Ks", m.specluer);

        if (op.usePower)
            WriteFloat1(ofs, "Ns", m.power);

        if (op.useAlpha)
            WriteFloat1(ofs, "d", m.alpha );

        if (op.useLum)
            WriteFloat1(ofs, "illum", m.lum);

        if (op.useTf)
            WriteFloat3(ofs, "Tf", m.Tf);

        if (op.useNi)
            WriteFloat1(ofs, "Ni", m.Ni);

        WritePath(ofs, "map_Kd", m.diffuseMap);
        WritePath(ofs, "map_Ka", m.ambientMap);
        WritePath(ofs, "map_Ks", m.specluerMap);
        WritePath(ofs, "map_Ns", m.hightLightMap);
        WritePath(ofs, "map_Bump", m.bumpMap);
        WritePath(ofs, "map_d", m.alphaMap);
    }
    return TS_TRUE;
}

//----------------------------------------------------------
//! 3Dモデルデータ(*.obj)を出力する
//----------------------------------------------------------
TsBool TsOBJLoader::SaveFile(const TsChar* filename)
{
    TsDebugLog("Save obj ->%s \n", filename);

    std::ofstream ofs( filename );

    //! Vector2 をストリーム出力する
    auto WriteFloat2 = [](std::ofstream& ofs, const TsChar* head, TsVector2& param)
    {
        ofs << head << " " << param.x << 
                       " " << param.y << std::endl;
    };

    //! Vector3 をストリーム出力する
    auto WriteFloat3 = [](std::ofstream& ofs, const TsChar* head, TsVector3& param)
    {
        ofs << head << " " << param.x << 
                       " " << param.y << 
                       " " << param.z << std::endl;
    };

    //! 引数の拡張子を変更してマテリアルファイルを出力する
    TSUT::TsFilePathAnalyzer analizer(filename);
    TSUT::TsFilePathAnalyzer materialAnalizer(filename);
    materialAnalizer.ReExtencion(".mtl");

    TsBool usingMaterial = m_materialList.size() > 0;

    if ( usingMaterial )
    {
        ofs << "mtllib" << " " << materialAnalizer.GetFileName() << std::endl;
        SaveMaterial(materialAnalizer.GetFullPath().c_str());
    }

    //位置の出力
    for (auto v : m_posList)
        WriteFloat3(ofs, "v", v);

    //法線の出力
    for (auto vn : m_normalList)
        WriteFloat3(ofs, "vn", vn);

    //uvの出力
    for (auto vt : m_texcoordList)
        WriteFloat2(ofs, "vt", vt);

    //面の構成要素を決定する
    TsByte flag = 0;
    flag |= m_posList.size() > 0 ? 1 : 0;
    flag |= m_normalList.size() > 0 ? 2 : 0;
    flag |= m_texcoordList.size() > 0 ? 4 : 0;

    for( auto m : m_objMesh )
    {
        //メッシュの名前があればグループとして出力
        if( m.name.size() > 0 )
        {
            ofs << "g" << " " << m.name << std::endl;
        }

        //materialを使用していれば使用しているマテリアルを出力
        if( m.material_name.size() > 0 )
        {
            ofs << "usemtl" << " " << m.material_name << std::endl;
        }
        for( auto f : m.m_face )
        {

            ofs << "f" << " ";

            //! 座標・法線・UVのパターン
            if( flag == 0x07 )
            {
                ofs << f.idx_pos.x << "/"
                    << f.idx_texcoord.x << "/"
                    << f.idx_normal.x << "/" << " "

                    << f.idx_pos.y << "/"
                    << f.idx_texcoord.y << "/"
                    << f.idx_normal.y << "/" << " "

                    << f.idx_pos.z << "/"
                    << f.idx_texcoord.z << "/"
                    << f.idx_normal.z << std::endl;
            }

            //! 座標・法線のパターン
            if( flag == 0x03 )
            {
                ofs << f.idx_pos.x << "/"
                    << f.idx_normal.x << " "

                    << f.idx_pos.y << "/"
                    << f.idx_normal.y << " "

                    << f.idx_pos.z << "/"
                    << f.idx_normal.z << std::endl;
            }
            //! 座標・UVのパターン
            if( flag == 0x05 )
            {
                ofs << f.idx_pos.x << "/"
                    << f.idx_texcoord.x << " "

                    << f.idx_pos.y << "/"
                    << f.idx_texcoord.y << " "

                    << f.idx_pos.z << "/"
                    << f.idx_texcoord.z << std::endl;
            }

            //! 座標のみのパターン
            if( flag == 1 )
            {
                ofs << f.idx_pos.x << " "
                    << f.idx_pos.y << " "
                    << f.idx_pos.z << std::endl;
            }
        }
    }
    return TS_TRUE;
}

//----------------------------------------------------------
//! 3Dモデルデータ(*.obj)を読み込む
//----------------------------------------------------------
TsBool TsOBJLoader::LoadObj(const TsChar* filename)
{
    std::ifstream ifs(filename);

    if (ifs.fail())
        return TS_FALSE;

    TSUT::TsFilePathAnalyzer analizer(filename);

    TsVector3 tempPos;
    TsVector3 tempNormal;
    TsVector2 tempUV;
    TsInt useMaterialIndex = -1;

    TsString buf;

    BYTE flag = 0;
    enum
    {
        USE_POS     = 0x01,
        USE_NORMAL  = 0x02,
        USE_UV      = 0x04,
    };

    auto ReadVec2 = [](const TsString& source, const TsChar* element, TsVector2& result)
    {
        TsInt strPos = TsString::npos;
        strPos = source.find(element);
        TsInt elementSize = TsString(element).size();
        if (strPos != TsString::npos)
        {
            sscanf_s(source.substr(strPos + elementSize).c_str(),
                "%f %f",
                &result.x,
                &result.y);
            return TS_TRUE;
        }
        return TS_FALSE;
    };

    auto ReadVec3 = [](const TsString& source, const TsChar* element, TsVector3& result)
    {
        TsInt strPos = TsString::npos;
        strPos = source.find(element);
        TsInt elementSize = TsString(element).size();
        if (strPos != TsString::npos)
        {
            sscanf_s(source.substr(strPos + elementSize).c_str(),
                "%f %f %f",
                &result.x,
                &result.y,
                &result.z);
            return TS_TRUE;
        }
        return TS_FALSE;
    };

    //------------------------------------------------------------
    // File Loop
    //------------------------------------------------------------
    while (ifs.eof() == TS_FALSE)
    {
        // Get File Line
        std::getline(ifs, buf);

        //------------------------------------------------------------
        // vertex postions
        //------------------------------------------------------------
        if ( ReadVec3(buf,"v ",tempPos))
        {
            m_posList.push_back(tempPos);
            flag |= USE_POS;
            continue;
        }

        //------------------------------------------------------------
        // vertex normal
        //------------------------------------------------------------
        if (ReadVec3(buf, "vn ", tempNormal))
        {
            m_normalList.push_back(tempNormal);
            flag |= USE_NORMAL;
            continue;
        }

        //------------------------------------------------------------
        // vertex texcoord
        //------------------------------------------------------------
        if (ReadVec2(buf, "vt ", tempUV))
        {
            m_texcoordList.push_back(tempUV);
            flag |= USE_UV;
            continue;
        }

        //------------------------------------------------------------
        // Load Mtl File
        //------------------------------------------------------------
        if (buf.find("mtllib ") == 0)
        {
            LoadMtl((analizer.GetLocalDirectory() + buf.substr(7)).c_str());
            continue;
        }

        //------------------------------------------------------------
        // Using material
        //------------------------------------------------------------
        if (buf.find("usemtl ") == 0)
        {
            if( m_objMesh.empty() )
                m_objMesh.push_back( TsObjMesh() );
            m_objMesh[m_objMesh.size()-1].material_name = buf.substr( 7 );
            continue;
        }

        if( buf.find( "g " ) == 0 )
        {
            m_objMesh.push_back( TsObjMesh() );
            m_objMesh[m_objMesh.size() - 1].name = buf.substr( 2 );
            if (m_objMesh.size() > 1)
                m_objMesh[m_objMesh.size() - 1].material_name = m_objMesh[m_objMesh.size() - 2].material_name;
            continue;
        }

        //------------------------------------------------------------
        // face 
        //------------------------------------------------------------
        if (buf.find("f ") == 0)
        {
            if( m_objMesh.empty() )
                m_objMesh.push_back( TsObjMesh() );
            TsVector<TsObjFace>& faceList = m_objMesh[m_objMesh.size() - 1].m_face;
            
            TsBool usePos = (flag&USE_POS) > 0;
            TsBool useNormal = (flag&USE_NORMAL) > 0;
            TsBool useUV = (flag&USE_UV) > 0;

            //------------------------------------------------------------
            //! Count of face controll points
            //------------------------------------------------------------
            TsInt indexCount = 0;
            TsChar oldBuffer = 0;
            for (UINT i = 0; i < buf.size(); ++i)
            {
                if (buf[i] != ' ' && oldBuffer == ' ')
                    indexCount++;
                oldBuffer = buf[i];
            }

            static const TsInt maxFace = 128;

            if (indexCount > maxFace)
            {
                TsDebugLogError("対応していない面の形式です。");
                return TS_FALSE;
            }

            //------------------------------------------------------------
            // face param = pos & normal & uv
            //------------------------------------------------------------
            if (usePos && useNormal && useUV)
            {
                // 区切り文字の置き換え
                std::replace(buf.begin()+2, buf.end(), '/', ' ');
                std::stringstream ss(&buf[2]);

                TsUint posIndexList[maxFace];
                TsUint nomIndexList[maxFace];
                TsUint uv0IndexList[maxFace];

                TsUint cv_posIndexList[maxFace * 3];
                TsUint cv_nomIndexList[maxFace * 3];
                TsUint cv_uv0IndexList[maxFace * 3];

                //データを抽出
                for (TsInt i = 0; i < indexCount; ++i)
                {
                   ss >> posIndexList[i] >>
                         uv0IndexList[i] >>
                         nomIndexList[i];                   
                }

                //トライアングルにコンバート
                TSUT::ConvertGeometoryToTriangleList(cv_nomIndexList, nomIndexList, indexCount);
                TSUT::ConvertGeometoryToTriangleList(cv_posIndexList, posIndexList, indexCount);
                TSUT::ConvertGeometoryToTriangleList(cv_uv0IndexList, uv0IndexList, indexCount);

                for (TsInt i = 0; i < indexCount - 2; ++i)
                {
                    TsObjFace f;

                    f.idx_pos[0] = cv_posIndexList[i*3 + 0];
                    f.idx_pos[1] = cv_posIndexList[i*3 + 1];
                    f.idx_pos[2] = cv_posIndexList[i*3 + 2];

                    f.idx_texcoord[0] = cv_uv0IndexList[i * 3 + 0];
                    f.idx_texcoord[1] = cv_uv0IndexList[i * 3 + 1];
                    f.idx_texcoord[2] = cv_uv0IndexList[i * 3 + 2];

                    f.idx_normal[0] = cv_nomIndexList[i * 3 + 0];
                    f.idx_normal[1] = cv_nomIndexList[i * 3 + 1];
                    f.idx_normal[2] = cv_nomIndexList[i * 3 + 2];

                    faceList.push_back(f);
                }
               
                continue;
            }

            //------------------------------------------------------------
            // face param = pos & normal
            //------------------------------------------------------------
            else if (usePos && useNormal)
            {
                // 区切り文字の置き換え

                if (buf.find("//") != TsString::npos)
                {
                    TsDebugLogError("区切り文字//はサポートされていません。");
                }

                std::replace(buf.begin() + 2, buf.end(), '/', ' ');

                //! ２つ以上空白がつながるケースは重複する空白を破棄する
                TsChar prev = 0;
                for (UINT i = 0; i < buf.size(); ++i)
                {
                    if (buf[i] == ' ' && prev == ' ')
                    {
                        buf.erase(i, 1);
                        i = 0;
                        prev = 0;
                        continue;
                    }
                    prev = buf[i];
                }

                std::stringstream ss(&buf[2]);

                TsUint posIndexList[maxFace];
                TsUint nomIndexList[maxFace];

                TsUint cv_posIndexList[maxFace * 3];
                TsUint cv_nomIndexList[maxFace * 3];

                //データを抽出
                for (TsInt i = 0; i < indexCount; ++i)
                {
                    ss >> posIndexList[i] >>
                        nomIndexList[i];
                }

                //多角形面をトライアングル面リストにコンバート
                TSUT::ConvertGeometoryToTriangleList(cv_nomIndexList, nomIndexList, indexCount);
                TSUT::ConvertGeometoryToTriangleList(cv_posIndexList, posIndexList, indexCount);

                for (TsInt i = 0; i < indexCount - 2; ++i)
                {
                    TsObjFace f;

                    f.idx_pos[0] = cv_posIndexList[i * 3 + 0];
                    f.idx_pos[1] = cv_posIndexList[i * 3 + 1];
                    f.idx_pos[2] = cv_posIndexList[i * 3 + 2];

                    f.idx_normal[0] = cv_nomIndexList[i * 3 + 0];
                    f.idx_normal[1] = cv_nomIndexList[i * 3 + 1];
                    f.idx_normal[2] = cv_nomIndexList[i * 3 + 2];

                    faceList.push_back(f);
                }

                continue;

            }
            //------------------------------------------------------------
            // face param = pos & uv
            //------------------------------------------------------------
            else if (usePos && useUV)
            {
                // 区切り文字の置き換え
                std::replace(buf.begin() + 2, buf.end(), '/', ' ');
                std::stringstream ss(&buf[2]);

                TsUint posIndexList[maxFace];
                TsUint uv0IndexList[maxFace];

                TsUint cv_posIndexList[maxFace * 3];
                TsUint cv_uv0IndexList[maxFace * 3];

                //データを抽出
                for (TsInt i = 0; i < indexCount; ++i)
                {
                    ss >> posIndexList[i] >>
                        uv0IndexList[i];
                }

                //多角形面をトライアングル面リストにコンバート
                TSUT::ConvertGeometoryToTriangleList(cv_posIndexList, posIndexList, indexCount);
                TSUT::ConvertGeometoryToTriangleList(cv_uv0IndexList, uv0IndexList, indexCount);

                for (TsInt i = 0; i < indexCount - 2; ++i)
                {
                    TsObjFace f;

                    f.idx_pos[0] = cv_posIndexList[i * 3 + 0];
                    f.idx_pos[1] = cv_posIndexList[i * 3 + 1];
                    f.idx_pos[2] = cv_posIndexList[i * 3 + 2];

                    f.idx_texcoord[0] = cv_uv0IndexList[i * 3 + 0];
                    f.idx_texcoord[1] = cv_uv0IndexList[i * 3 + 1];
                    f.idx_texcoord[2] = cv_uv0IndexList[i * 3 + 2];

                    faceList.push_back(f);
                }

                continue;
            }

            //------------------------------------------------------------
            // face param = pos only
            //------------------------------------------------------------
            else if (usePos)
            {
                // 区切り文字の置き換え
                std::stringstream ss(&buf[2]);

                TsUint posIndexList[maxFace];

                TsUint cv_posIndexList[maxFace * 3];

                //データを抽出
                for (TsInt i = 0; i < indexCount; ++i)
                {
                    ss >> posIndexList[i];
                }

                //多角形面をトライアングル面リストにコンバート
                TSUT::ConvertGeometoryToTriangleList(cv_posIndexList, posIndexList, indexCount);

                for (TsInt i = 0; i < indexCount - 2; ++i)
                {
                    TsObjFace f;

                    f.idx_pos[0] = cv_posIndexList[i * 3 + 0];
                    f.idx_pos[1] = cv_posIndexList[i * 3 + 1];
                    f.idx_pos[2] = cv_posIndexList[i * 3 + 2];

                    faceList.push_back(f);
                }

                continue;
            }
            else
            {
                TsDebugLogError("対応していない面の形式です。");
                return TS_FALSE;
            }
        }

    }
    return TS_TRUE;
}

//----------------------------------------------------------
//! materialデータ(*.mtl)を読み込む
//----------------------------------------------------------
TsBool TsOBJLoader::LoadMtl(const TsChar* filename)
{
    Ts3DModelLoader::LoadFile( filename );
    std::ifstream ifs(filename);

    if (ifs.is_open() == TS_FALSE)
    {
        TsDebugLogError("Load Faild \n\t\"%s\"",filename);
        return TS_FALSE;
    }
    TSUT::TsFilePathAnalyzer analizer(filename);

    auto ReadFloat1 = [](const TsString& source, const TsChar* element, TsF32& result)
    {
        TsInt strPos = TsString::npos;
        strPos = source.find(element);
        TsInt elementSize = TsString(element).size();
        if (strPos == 0)
        {
            sscanf_s(source.substr(strPos + elementSize).c_str(),
                "%f",&result);
            return TS_TRUE;
        }
        return TS_FALSE;
    };

    auto ReadFloat3 = [](const TsString& source, const TsChar* element, TsFloat3& result)
    {
        TsInt strPos = TsString::npos;
        strPos = source.find(element);
        TsInt elementSize = TsString(element).size();
        if (strPos == 0)
        {
            sscanf_s(source.substr(strPos + elementSize).c_str(),
                "%f %f %f",
                &result.x,
                &result.y,
                &result.z);
            return TS_TRUE;
        }
        return TS_FALSE;
    };

    auto ReadTexPath = [&](const TsString& source, const TsChar* element, TsString& result)
    {
        TsInt strPos = TsString::npos;
        strPos = source.find(element);
        TsInt elementSize = TsString(element).size();
        if (strPos != TsString::npos)
        {
            result = source.substr(source.rfind(0x20)+1);
            return TS_TRUE;
        }
        return TS_FALSE;
    };

    TsString buf;

    //------------------------------------------------------------
    // Loop Material File
    //------------------------------------------------------------
    while (ifs.eof() == TS_FALSE)
    {
        // Get File Line
        std::getline(ifs, buf);

        //------------------------------------------------------------
        // Material
        //------------------------------------------------------------
        if (buf.find("newmtl") != TsString::npos)
        {
            TsObjMaterial temp;
            temp.name = buf.substr(7);
            temp.dir = analizer.GetLocalDirectory();

            //------------------------------------------------------------
            // Load Material Parameters
            //------------------------------------------------------------
            while( ifs.eof() == TS_FALSE &&
                buf != "")
            {
                std::getline(ifs, buf);
                // diffuse
                if (ReadFloat3(buf, "Kd ", temp.diffuse))
                {
                    temp.usingMaterialParam.useDiffuse = TS_TRUE;
                    continue;
                }

                // ambient
                if( ReadFloat3(buf, "Ka ", temp.ambient))
                {
                    temp.usingMaterialParam.useAmbient = TS_TRUE;
                    continue;
                }

                // speculer
                if (ReadFloat3(buf, "Ks ", temp.specluer))
                {
                    temp.usingMaterialParam.useSpecluer = TS_TRUE;
                    continue;
                }

                // seculerPower
                if( ReadFloat1(buf, "Ns ", temp.power))
                {
                    temp.usingMaterialParam.usePower = TS_TRUE;
                    continue;
                }

                // alpha
                {
                    if (ReadFloat1(buf, "d ", temp.alpha))
                    {
                        temp.usingMaterialParam.useAlpha = TS_TRUE;
                        continue;
                    }
                    // is revers alpha ?
                    else if (ReadFloat1(buf, "Tr ", temp.alpha))
                    {
                        temp.alpha = 1 - temp.alpha;
                        temp.usingMaterialParam.useAlpha = TS_TRUE;
                        continue;
                    }
                }

                // Tf ?
                if (ReadFloat3(buf, "Tf ", temp.Tf))
                {
                    temp.usingMaterialParam.useTf = TS_TRUE;
                    continue;
                }

                // Ni ?
                if (ReadFloat1(buf, "Ni ", temp.Ni))
                {
                    temp.usingMaterialParam.useNi = TS_TRUE;
                    continue;
                }
                // lumminus 
                if (ReadFloat1(buf, "illum ", temp.lum))
                {
                    temp.usingMaterialParam.useLum = TS_TRUE;
                    continue;
                }

                // diffuse texture
                if (ReadTexPath(buf, "map_Kd ", temp.diffuseMap))
                {
                    continue;
                }

                // ambient texture
                if( ReadTexPath(buf, "map_Ka ", temp.ambientMap))
                {
                    continue;
                }
                // speculer texture
                if (ReadTexPath(buf, "map_Ks ", temp.specluerMap))
                {
                    continue;
                }

                // hight light texture
                if (ReadTexPath(buf, "map_Ns ", temp.hightLightMap))
                {
                    continue;
                }

                // normal texture
                if (ReadTexPath(buf, "map_Bump ", temp.bumpMap))
                {
                    continue;
                }
                
                if (ReadTexPath(buf, "Bump ", temp.bumpMap))
                {
                    continue;
                }

                // hight light texture
               if( ReadTexPath(buf, "map_d ", temp.alphaMap))
                {
                    continue;
                }

            }// End while
            m_materialList.push_back(temp);
        }// End if
    }//End while
    return TS_TRUE;
}

//----------------------------------------------------------
//! 独自メッシュデータを[*.obj]で保存できるようにエンコードする
//----------------------------------------------------------
TsBool TsOBJLoader::Encode(TsMeshObject* pMesh)
{
    m_objMesh.resize(pMesh->GetGeometryCount());
    m_meshCount = m_objMesh.size();
    TsInt startLocation = 1;
    for (TsInt i = 0; i < m_objMesh.size(); ++i)
    {
        auto * pElement = pMesh->GetGeometry(i)->GetVertexElement();
        TsVertexSkin* pVertexList = (TsVertexSkin*)pElement->GetVertexBuffer()->GetOriginData();
        TsUint*       pIndexData = (TsUint*)pElement->GetIndexBuffer()->GetOriginData();

        //----------------------------------------------------
        // material 
        //----------------------------------------------------
        TsDefaultMaterial* pMat = (TsDefaultMaterial*)pMesh->GetGeometry(i)->GetMaterial();
        {
            m_objMesh[i].name = pMesh->GetGeometry(i)->GetName();
            m_objMesh[i].material_name = pMat->GetName();

            TsObjMaterial mat;

            mat.name = pMat->GetName();

            mat.diffuse.x = pMat->GetColor().x;
            mat.diffuse.y = pMat->GetColor().y;
            mat.diffuse.z = pMat->GetColor().z;
            mat.alpha = pMat->GetColor().w;

            mat.ambient.x = pMat->GetAmbient().x;
            mat.ambient.y = pMat->GetAmbient().y;
            mat.ambient.z = pMat->GetAmbient().z;

            mat.specluer.x = pMat->GetSpecluer().x;
            mat.specluer.y = pMat->GetSpecluer().y;
            mat.specluer.z = pMat->GetSpecluer().z;

            mat.power = pMat->GetSpeculerPower();

            if (pMat->GetAlbedoTexture())
            {
                TSUT::TsFilePathAnalyzer ana (pMat->GetAlbedoTexture()->GetName());
                mat.diffuseMap = ana.GetFileName();
            }

            if (pMat->GetSpeculerTexture())
            {
                TSUT::TsFilePathAnalyzer ana(pMat->GetSpeculerTexture()->GetName());
                mat.specluerMap = ana.GetFileName();
            }

            if (pMat->GetNormalTexture())
            {
                TSUT::TsFilePathAnalyzer ana(pMat->GetNormalTexture()->GetName());
                mat.bumpMap = ana.GetFileName();
            }

            mat.usingMaterialParam.useAlpha     = mat.alpha < 1.0f;
            mat.usingMaterialParam.useAmbient   = 
            mat.usingMaterialParam.useDiffuse   = 
            mat.usingMaterialParam.useSpecluer  = 
            mat.usingMaterialParam.usePower     = TS_TRUE;

            m_materialList.push_back(mat);
        }
        m_materialCount = m_materialList.size();

        startLocation = m_posList.size() + 1;
        for (TsInt j = 0;  j < pElement->GetVertexNum(); ++j)
        {
            //! 左手系 - > 右手系　の変換
            TsVector3 p, n;
            TsVector2 uv;
            p = pVertexList[j].pos;
            p.x *= -1;

            n = pVertexList[j].normal;
            n.x *= -1;

            uv = pVertexList[j].uv;
            uv.y = 1 - uv.y;


            m_posList.push_back(p);

            if (n.Length() > 0.9f)
                m_normalList.push_back(n);

            if (pMat->GetAlbedoTexture() || pMat->GetSpeculerTexture() || pMat->GetNormalTexture() )
                m_texcoordList.push_back(uv);

        }


        //! IndexBufferを使っていない場合は頂点リストの並びをそのまま面の構成情報とする
        if (pElement->GetIndexNum() == 0)
        {
            for (TsInt j = 0; pElement->GetVertexNum() / 3; ++j)
            {
                TsObjFace f;
                f.idx_pos[0] = (j * 3) + 2 + startLocation;
                f.idx_pos[1] = (j * 3) + 1 + startLocation;
                f.idx_pos[2] = (j * 3) + 0 + startLocation;

                f.idx_normal[0] = (j * 3) + 2 + startLocation;
                f.idx_normal[1] = (j * 3) + 1 + startLocation;
                f.idx_normal[2] = (j * 3) + 0 + startLocation;

                f.idx_texcoord[0] = (j * 3) + 2 + startLocation;
                f.idx_texcoord[1] = (j * 3) + 1 + startLocation;
                f.idx_texcoord[2] = (j * 3) + 0 + startLocation;

                m_objMesh[i].m_face.push_back(f);
            }
        }
        else
        {
            for (TsInt j = 0; j < pElement->GetIndexNum() / 3; ++j)
            {
                TsUint* idx0 = &((TsUint*)(pElement->GetIndexBuffer()->GetOriginData()))[j*3];
                TsObjFace f;
                f.idx_pos[0] = *(idx0+2) + +startLocation;
                f.idx_pos[1] = *(idx0+1) + startLocation;
                f.idx_pos[2] = *(idx0+0) + startLocation;

                f.idx_normal[0] = *(idx0 + 2) + +startLocation;
                f.idx_normal[1] = *(idx0 + 1) + startLocation;
                f.idx_normal[2] = *(idx0 + 0) + startLocation;

                f.idx_texcoord[0] = *(idx0 + 2) + +startLocation;
                f.idx_texcoord[1] = *(idx0 + 1) + startLocation;
                f.idx_texcoord[2] = *(idx0 + 0) + startLocation;

                m_objMesh[i].m_face.push_back(f);
            }
        }              
    }
    return TS_TRUE;
}

//----------------------------------------------------------
//! *.objのデータ構造から独自メッシュに変換するための
//    中間フォーマットに変換する
//----------------------------------------------------------
TsBool TsOBJLoader::CreateCommonData()
{
    m_pMeshs = TsNew( TsCommon3DMesh[m_objMesh.size()] );
    m_meshCount = m_objMesh.size();

    TSUT::TsFilePathAnalyzer analizer( GetName() );
    analizer.ReExtencion( "" );

    if( m_materialList.size() == 0 )
    {
        m_materialCount = 1;
        m_pMaterials = TsNew( TsCommon3DMaterial );
        m_pMaterials->m_name = analizer.GetFileName() + ":NullMaterial";
    }
    else
    {
        m_materialCount = m_materialList.size() + 1;
        m_pMaterials = TsNew( TsCommon3DMaterial[m_materialCount] );
        m_pMaterials[0].m_name = analizer.GetFileName() + ":NullMaterial";

        for( TsInt i = 1; i < m_materialCount; ++i )
        {
            auto & cm = m_pMaterials[i];
            auto & or = m_materialList[i-1];
            cm.m_diffuse = or.diffuse;
            cm.m_alpha = or.alpha;
            cm.m_ambient = or.ambient;
            cm.m_name = or.name;
            cm.m_power = or.power;
            cm.m_specluer = or.specluer;
            cm.m_albedoTexture = or.diffuseMap;
            cm.m_specluerTexture = or.specluerMap;
            cm.m_normalTexture = or.bumpMap;
            cm.m_specluerPowerTexture = "";
        }
    }

    for( TsInt mId =0; mId < m_meshCount; ++mId )
    {
        auto& fList = m_objMesh[mId].m_face;
        m_pMeshs[mId].m_name = analizer.GetFileName() +":" + m_objMesh[mId].name;

        if( m_objMesh[mId].material_name.size() == 0 )
        {
            m_pMeshs[mId].m_pMaterialRef = &m_pMaterials[0];
        }
        else
        {
            for( TsInt i = 1; i < m_materialCount; ++i )
            {
                if( m_objMesh[mId].material_name == m_pMaterials[i].m_name )
                {
                    m_pMeshs[mId].m_pMaterialRef = &m_pMaterials[i];
                }
            }
            if( m_pMeshs[mId].m_pMaterialRef == nullptr )
            {
                m_pMeshs[mId].m_pMaterialRef = &m_pMaterials[0];
            }
        }

        m_pMeshs[mId].m_vertexCount = fList.size() * 3;
        m_pMeshs[mId].m_pTransoform = TsNew( TsTransForm );
        m_pMeshs[mId].m_pPositions = TsNew( TsVector3[fList.size() * 3] );
        if( m_normalList.size() >0 )
            m_pMeshs[mId].m_pNormals = TsNew( TsVector3[fList.size() * 3] );
        if( m_texcoordList.size() >0 )
            m_pMeshs[mId].m_pTexcoords = TsNew( TsVector2[fList.size() * 3] );
        for( TsUint i = 0; i < fList.size(); ++i )
            for( TsUint j = 0; j < 3; ++j )
            {
                TsUint index = ( i * 3 ) + ( j );      //インデックス
                TsUint invIndex = ( 2 - j );           //右手系 -> 左手系の　面変換用

                TsUint pIDx = fList[i].idx_pos[invIndex] -1;
                TsUint nIdx = fList[i].idx_normal[invIndex] -1;
                TsUint tIdx = fList[i].idx_texcoord[invIndex] -1;

                TsVector3 & p = m_pMeshs[mId].m_pPositions[index];
                p = m_posList[pIDx];
                p.x *= -1;

                if( m_normalList.size() > 0 )
                {
                    TsVector3 & n = m_pMeshs[mId].m_pNormals[index];
                    n = m_normalList[nIdx];
                    n.x *= -1;
                }

                if( m_texcoordList.size() > 0 )
                {
                    TsVector2 & t = m_pMeshs[mId].m_pTexcoords[index];
                    t = m_texcoordList[tIdx];
                    t.y = 1 - t.y;
                }

                //! AABBの作成
                m_pMeshs[mId].m_aabb.AddPoint( p );
            }
    }
    return TS_TRUE;
}