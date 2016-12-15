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
size_t GetVertexStride()
{
    return 0;
}

TsBool TsOBJLoader::SaveFile(const char* filename)
{
    //todo
    return 1;
}

TsBool TsOBJLoader::LoadObj(const char* filename)
{
    TsDebugLog("Load obj ->%s", filename);
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
        if (ReadVec2(buf, "vn ", tempUV))
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
            currentMaterial = buf.substr(8);
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

    TsDebugLog("Load mtl ->%s", filename);

    if (ifs.is_open() == false)
    {
        OutputDebugString("ObjLoader::Load Mtl File Open Error");
        return false;
    }

    auto ReadFloat1 = [](const TsString& source, const char* element, TsF32& result)
    {
        TsInt strPos = TsString::npos;
        strPos = source.find(element);
        TsInt elementSize = TsString(element).size();
        if (strPos != TsString::npos)
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

    auto ReadTexPath = [](const TsString& source, const char* element, TsString& result)
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

    TSUT::TsFilePathAnalyzer analizer(filename);

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
        if (buf.find("newmtl"))
        {
            TsObjMaterial temp;
            temp.name = buf.substr(7);

            //------------------------------------------------------------
            // Load Material Parameters
            //------------------------------------------------------------
            while (ifs.eof() == false &&
                buf[0] != '\n')
            {
                std::getline(ifs, buf);
                // diffuse
                ReadFloat3(buf, "Kd ", temp.diffuse);

                // ambient
                ReadFloat3(buf, "Ka ", temp.ambient);

                // speculer
                ReadFloat3(buf, "Ks ", temp.specluer);

                // seculerPower
                ReadFloat1(buf, "Ns ", temp.power);

                // alpha
                {
                    ReadFloat1(buf, "d ", temp.alpha);

                    // is revers alpha ?
                    if (ReadFloat1(buf, "Tr ", temp.alpha))
                    {
                        temp.alpha = 1 - temp.alpha;
                    }
                }

                // Tf ?
                ReadFloat3(buf, "Tf ", temp.Tf);

                // Ni ?
                ReadFloat1(buf, "Tf ", temp.Ni);

                // lumminus 
                ReadFloat1(buf, "illum ", temp.lum);

                // diffuse texture
                ReadTexPath(buf, "map_Kd ", temp.diffuseMap);

                // ambient texture
                ReadTexPath(buf, "map_Ka ", temp.ambientMap);

                // speculer texture
                ReadTexPath(buf, "map_Ks ", temp.specluerMap);

                // hight light texture
                ReadTexPath(buf, "map_Ns ", temp.hightLightMap);

                // normal texture
                ReadTexPath(buf, "map_Bump ", temp.bumpMap);
                ReadTexPath(buf, "Bump ", temp.bumpMap);

                // hight light texture
                ReadTexPath(buf, "map_d ", temp.alphaMap);

            }// End while
            m_materialList.push_back(temp);
        }// End if
    }//End while
    return true;
}