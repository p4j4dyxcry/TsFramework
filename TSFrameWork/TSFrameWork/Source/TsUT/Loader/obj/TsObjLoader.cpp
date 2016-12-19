#include "../../../../TsAfx.h"
#include <fstream>
TsOBJLoader::TsOBJLoader(){}
TsOBJLoader::~TsOBJLoader(){}
TsBool TsOBJLoader::LoadFromFile(const TsString& filename, TsLoadOption& option )
{
    (void)option;
    return LoadObj(filename.c_str());
}
TsBool TsOBJLoader::LoadFromMemory(void * memory, size_t sz)
{
    return TS_FALSE;
}
TsInt  TsOBJLoader::GetMeshNum()
{
    return 0;
}
TsInt TsOBJLoader::GetVertexSize(TsInt index)
{
    return 0;
}
void*  TsOBJLoader::GetVertexBuffer(TsInt index)
{
    return 0;
}
size_t TsOBJLoader::GetVertexStride()
{
    return 0;
}

TsBool TsOBJLoader::SaveMaterial(const char* filename)
{
    std::ofstream ofs(filename);

    auto WriteFloat1 = [](std::ofstream& ofs, const char* head, TsF32 param)
    {
        ofs << head << " " << param << std::endl;
    };

    auto WriteFloat3 = [](std::ofstream& ofs, const char* head, TsFloat3& param)
    {
        ofs << head << " " << param.x << 
                       " " << param.y << 
                       " " << param.z << std::endl;
    };

    auto WritePath = [](std::ofstream& ofs, const char* head, TsString& param)
    {
        if (param.size() > 0)
            ofs << head << " " << param << std::endl;
    };

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

TsBool TsOBJLoader::SaveFile(const char* filename)
{
    TsDebugLog("Save obj ->%s \n", filename);

    std::ofstream ofs( filename );

    auto WriteFloat2 = [](std::ofstream& ofs, const char* head, TsVector2& param)
    {
        ofs << head << " " << param.x << 
                       " " << param.y << std::endl;
    };

    auto WriteFloat3 = [](std::ofstream& ofs, const char* head, TsVector3& param)
    {
        ofs << head << " " << param.x << 
                       " " << param.y << 
                       " " << param.z << std::endl;
    };

    TSUT::TsFilePathAnalyzer analizer(filename);
    TSUT::TsFilePathAnalyzer materialAnalizer(filename);
    materialAnalizer.ReExtencion(".mtl");

    TsBool usingMaterial = m_materialList.size() > 0;

    if ( usingMaterial )
    {
        ofs << "mtllib" << " " << materialAnalizer.GetFileName() << std::endl;
        SaveMaterial(materialAnalizer.GetFullPath().c_str());
    }

    for (auto v : m_posList)
        WriteFloat3(ofs, "v", v);

    for (auto vn : m_normalList)
        WriteFloat3(ofs, "vn", vn);

    for (auto vt : m_texcoordList)
        WriteFloat2(ofs, "vt", vt);

    TsByte flag = 0;
    flag |= m_posList.size() > 0 ? 1 : 0;
    flag |= m_normalList.size() > 0 ? 2 : 0;
    flag |= m_texcoordList.size() > 0 ? 4 : 0;
    TsString currentMaterial = "";

    for (auto f : m_faceList)
    {
        if (currentMaterial != f.material_name)
        {
            ofs << "usemtl" << " " << f.material_name << std::endl;
            currentMaterial = f.material_name;
        }
        ofs << "f" << " ";
        if (flag == 0x07)
        {
            ofs << f.idx_pos.x      << "/"
                << f.idx_texcoord.x << "/"
                << f.idx_normal.x   << "/" << " "

                << f.idx_pos.y      << "/"
                << f.idx_texcoord.y << "/"
                << f.idx_normal.y   << "/" << " "

                << f.idx_pos.z      << "/"
                << f.idx_texcoord.z << "/"
                << f.idx_normal.z   <<  std::endl;
        }

        if (flag == 0x03)
        {
            ofs << f.idx_pos.x      << "/"
                << f.idx_normal.x   << " "

                << f.idx_pos.y      << "/"
                << f.idx_normal.y   << " "

                << f.idx_pos.z      << "/"
                << f.idx_normal.z   <<  std::endl;
        }

        if (flag == 0x05)
        {
            ofs << f.idx_pos.x      << "/"
                << f.idx_texcoord.x << " "

                << f.idx_pos.y      << "/"
                << f.idx_texcoord.y << " "

                << f.idx_pos.z      << "/"
                << f.idx_texcoord.z <<  std::endl;
        }

        if (flag == 1)
        {
            ofs << f.idx_pos.x << " "
                << f.idx_pos.y << " "
                << f.idx_pos.z << std::endl;
        }

    }

    return 1;
}

TsBool TsOBJLoader::LoadObj(const char* filename)
{
    TsDebugLog("Load obj ->%s \n", filename);
    std::ifstream ifs(filename);

    if (ifs.fail())
        return TS_FALSE;

    TSUT::TsFilePathAnalyzer analizer(filename);

    TsVector3 tempPos;
    TsVector3 tempNormal;
    TsVector2 tempUV;
    int useMaterialIndex = -1;

    TsString buf;
    TsString currentMaterial;

    BYTE flag = 0;
    enum
    {
        USE_POS     = 0x01,
        USE_NORMAL  = 0x02,
        USE_UV      = 0x04,
    };

    auto ReadVec2 = [](const TsString& source, const char* element, TsVector2& result)
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

    auto ReadVec3 = [](const TsString& source, const char* element, TsVector3& result)
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
    while (ifs.eof() == false)
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
            flag |= USE_POS;
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
            currentMaterial = buf.substr(7);
            continue;
        }

        //------------------------------------------------------------
        // face 
        //------------------------------------------------------------
        if (buf.find("f ") == 0)
        {
            TsObjFace f;
            bool usePos = (flag&USE_POS) > 0;
            bool useNormal = (flag&USE_NORMAL) > 0;
            bool useUV = (flag&USE_UV) > 0;

            f.material_name = currentMaterial;

            //------------------------------------------------------------
            //! Count of face controll points
            //------------------------------------------------------------
            int indexCount = 0;
            for (UINT i = 0; i < buf.size(); ++i)
            {
                if (buf[i] == ' ')
                    indexCount++;
            }

            //------------------------------------------------------------
            // triangle face only
            //------------------------------------------------------------
            if (indexCount != 3)
            {
                TsDebugLog("対応していない面の形式です。");
                return TS_FALSE;
            }

            //------------------------------------------------------------
            // face param = pos & normal & uv
            //------------------------------------------------------------
            if (usePos && useNormal && useUV)
            {
                if (sscanf_s(&buf[2], "%d/%d/%d %d/%d/%d %d/%d/%d",
                    &f.idx_pos[0],
                    &f.idx_texcoord[0],
                    &f.idx_normal[0],
                    &f.idx_pos[1],
                    &f.idx_texcoord[1],
                    &f.idx_normal[1],
                    &f.idx_pos[2],
                    &f.idx_texcoord[2],
                    &f.idx_normal[2]) != 9)
                {
                    TsDebugLog("対応していない面の形式です。");
                    return TS_FALSE;
                }
                m_faceList.push_back(f);
                continue;
            }

            //------------------------------------------------------------
            // face param = pos & normal
            //------------------------------------------------------------
            else if (usePos && useNormal)
            {
                if (sscanf_s(&buf[2], "%d/%d %d/%d %d/%d",
                    &f.idx_pos[0],
                    &f.idx_normal[0],
                    &f.idx_pos[1],
                    &f.idx_normal[1],
                    &f.idx_pos[2],
                    &f.idx_normal[2]) != 6)
                {
                    TsDebugLog("対応していない面の形式です。");
                    return TS_FALSE;
                }
                m_faceList.push_back(f);
                continue;

            }
            //------------------------------------------------------------
            // face param = pos & uv
            //------------------------------------------------------------
            else if (usePos && useUV)
            {
                if (sscanf_s(&buf[2], "%d/%d %d/%d %d/%d",
                    &f.idx_pos[0],
                    &f.idx_texcoord[0],
                    &f.idx_pos[1],
                    &f.idx_texcoord[1],
                    &f.idx_pos[2],
                    &f.idx_texcoord[2]) != 6)
                {
                    TsDebugLog("対応していない面の形式です。");
                    return TS_FALSE;

                }
                m_faceList.push_back(f);
                continue;
            }

            //------------------------------------------------------------
            // face param = pos only
            //------------------------------------------------------------
            else if (usePos)
            {
                if (sscanf_s(&buf[2], "%d %d %d",
                    &f.idx_pos[0],
                    &f.idx_pos[1],
                    &f.idx_pos[2]) != 3)
                {
                    TsDebugLog("対応していない面の形式です。");
                    return TS_FALSE;
                }
                m_faceList.push_back(f);
                continue;
            }
            else
            {
                TsDebugLog("対応していない面の形式です。");
                return TS_FALSE;
            }
        }

    }
    return TS_TRUE;
}
TsBool TsOBJLoader::LoadMtl(const char* filename)
{
    std::ifstream ifs(filename);

    TsDebugLog("Load mtl ->%s \n", filename);

    if (ifs.is_open() == false)
    {
        OutputDebugString("ObjLoader::Load Mtl File Open Error");
        return false;
    }
    TSUT::TsFilePathAnalyzer analizer(filename);

    auto ReadFloat1 = [](const TsString& source, const char* element, TsF32& result)
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

    auto ReadFloat3 = [](const TsString& source, const char* element, TsFloat3& result)
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

    auto ReadTexPath = [&](const TsString& source, const char* element, TsString& result)
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
    while (ifs.eof() == false)
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
            while (ifs.eof() == false &&
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
    return true;
}