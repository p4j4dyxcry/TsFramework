#include "../../../../TsAfx.h"
#include <fstream>
TsStlLoader::TsStlLoader():
m_faceList(nullptr){}
TsStlLoader:: ~TsStlLoader()
{
    TsSafeDelete(m_faceList);
}

TsBool TsStlLoader::LoadFile(const TsChar* filename)
{
    Ts3DModelLoader::LoadFile(filename);
    m_filename = filename;

    if (IsBinary(filename,65535))
        return LoadFromBinary(filename);
    else
        return  LoadFromAscii(filename);
}

TsBool TsStlLoader::SaveFile(const TsChar* filename)
{
    Ts3DModelLoader::SaveFile(filename);

    if (m_isBinarySave)
        return SaveBinary(filename);
    else
        return SaveAscii(filename);
}

void TsStlLoader::SetBinarySaveFlag(TsBool flag)
{
    m_isBinarySave = flag;
}

//----------------------------------------------------------
//! バイナリ形式でstlファイルを保存する
//----------------------------------------------------------
TsBool TsStlLoader::SaveBinary(const TsChar* filename)
{
    std::ofstream ofs(filename,std::ios::binary);

    if (ofs.fail() || m_faceList == nullptr)
        return TS_FALSE;

    TsChar header[80] = "TsFramework stl Binary Format";

    ofs.write(header, sizeof(header));

    TsUint s = sizeof(TsStlFace);
    ofs.write((TsChar*)&m_faceSize, sizeof(UINT));
    ofs.write((TsChar*)m_faceList, sizeof(TsStlFace) * m_faceSize);

    return TS_TRUE;
}

//----------------------------------------------------------
//! バイナリ形式のstlファイルを読み込む
//----------------------------------------------------------
TsBool TsStlLoader::LoadFromBinary( const TsChar * filename )
{
    std::ifstream ifs(filename, std::ios::binary);

    if (ifs.fail())
        return TS_FALSE;

    TsChar header[80];

    ifs.read(header, sizeof(header));

    ifs.read((TsChar*)&m_faceSize, sizeof(TsUint));

    if (m_faceSize > 0)
    {
        TsSafeDelete(m_faceList);
        m_faceList = TsNew(TsStlFace[m_faceSize]);
    }
    else
    {
        return TS_FALSE;
    }

    ifs.read((TsChar*)m_faceList, sizeof(TsStlFace) * m_faceSize);

    return true;
}

//----------------------------------------------------------
//! オリジンメッシュから*.stl形式にデータをコンバートする
//----------------------------------------------------------
TsBool TsStlLoader::Encode(TsMeshObject* pMesh)
{
    TsVector<TsStlFace> faceList;

    for (auto p : pMesh->GetGeometrys())
    {
        TsVertexSkin* pVB = (TsVertexSkin*)p->GetVertexElement()->GetVertexBuffer()->GetOriginData();
        TsUint* pIB = (TsUint*)p->GetVertexElement()->GetIndexBuffer()->GetOriginData();
        TsUint verexCount = p->GetVertexElement()->GetVertexNum();
        TsUint indexCount = p->GetVertexElement()->GetIndexNum();

        if (indexCount > 0)
        {
            for (TsUint i = 0; i < indexCount / 3; ++i)
            {
                TsStlFace f;
                f.pos[2] = pVB[pIB[i * 3 + 0]].pos;
                f.pos[1] = pVB[pIB[i * 3 + 1]].pos;
                f.pos[0] = pVB[pIB[i * 3 + 2]].pos;

                f.normal = TsComputeFaceNormal(f.pos[2], f.pos[1], f.pos[0]);

                f.pos[0].x *= -1;
                f.pos[1].x *= -1;
                f.pos[2].x *= -1;
                f.normal.x *= -1;

                faceList.push_back(f);
            }
        }
        else
        {
            for (TsUint i = 0; i < verexCount / 3; ++i)
            {
                TsStlFace f;
                f.pos[2] = pVB[i * 3 + 0].pos;
                f.pos[1] = pVB[i * 3 + 1].pos;
                f.pos[0] = pVB[i * 3 + 2].pos;

                f.normal = TsComputeFaceNormal(f.pos[2], f.pos[1], f.pos[0]);

                f.pos[0].x *= -1;
                f.pos[1].x *= -1;
                f.pos[2].x *= -1;
                f.normal.x *= -1;

                faceList.push_back(f);
            }
        }
    }
    m_faceList = TsNew(TsStlFace[faceList.size()]);
    m_faceSize = faceList.size();
    memcpy(m_faceList, &faceList[0], sizeof(TsStlFace) * m_faceSize);
    return TS_TRUE;
}

//----------------------------------------------------------
//! Asciiコードでstlファイルを保存する
//----------------------------------------------------------
TsBool TsStlLoader::SaveAscii( const TsChar* filename )
{
    std::ofstream ofs(filename);

    if (ofs.fail() || m_faceList == nullptr)
        return TS_FALSE;

    // ヘッダ
    ofs << "solid TsFramework stl Ascii Format" << std::endl;
    for (TsUint i = 0; i < m_faceSize; ++i)
    {
        TsStlFace& f = m_faceList[i];

        ofs << "facet normal" << " "
            << f.normal.x << " "
            << f.normal.y << " "
            << f.normal.z << std::endl;

        ofs << "outer loop" << std::endl;

        for (TsUint i = 0; i < 3; ++i)
        {
            ofs << "vertex" << " "
                << f.pos[i].x << " "
                << f.pos[i].y << " "
                << f.pos[i].z << std::endl;
        }

        ofs << "endloop" << std::endl;
        ofs << "endfacet" << std::endl;
    }
    ofs << "end solid " << std::endl;
    return TS_TRUE;
};

//----------------------------------------------------------
//! Asciiコードのstlファイルを読み込む
//----------------------------------------------------------
TsBool TsStlLoader::LoadFromAscii( const TsChar * filename )
{
    std::ifstream ifs(filename);

    if (ifs.fail())
        return TS_FALSE;

    // Streamからベクトルを読み込む関数オブジェクト
    auto ReadVector3 = [](TsString& source ,const TsChar * element, TsVector3& v )
    {
        TsInt strPos = TsString::npos;
        strPos = source.find(element);
        TsInt elementSize = TsString(element).size();
        if (strPos != TsString::npos)
        {
            sscanf_s(source.substr(strPos + elementSize).c_str(),
                "%f %f %f",
                &v.x,
                &v.y,
                &v.z);
            return TS_TRUE;
        }
        return TS_FALSE;
    };

    TsString str;
    TsVector<TsStlFace> faceList;

    while (ifs.eof() == false)
    {
        std::getline(ifs,str);
        TsStlFace tempFace;
        
        // Read Normal
        if (ReadVector3(str, "facet normal ", tempFace.normal))
        {
            TsInt vertexNum = 0;
            // Read Vertex
            while (vertexNum != 3)
            {
                std::getline(ifs, str);
                if (ReadVector3(str, "vertex ", tempFace.pos[vertexNum]))
                {
                    vertexNum++;
                }
            }
            faceList.push_back(tempFace);
        }
    }
    m_faceSize = faceList.size();
    if (m_faceSize == 0)
    {
        TsSafeDelete(m_faceList);
        return TS_FALSE;
    }

    TsSafeDelete(m_faceList);
    m_faceList = TsNew(TsStlFace[m_faceSize]);
    memcpy(m_faceList, &faceList[0], sizeof(TsStlFace) * m_faceSize);

    return TS_TRUE;

}

//----------------------------------------------------------
//! stlファイルがバイナリか調べる
//----------------------------------------------------------
TsBool TsStlLoader::IsBinary( const TsChar* filename , TsInt nByte )
{
 
    std::ifstream ifs(filename, std::ios::binary);

    if (ifs.fail())
        return TS_FALSE;

    TsInt 	c;
    // nバイト探索しバイナリコードが見つかればバイナリとする
    for (TsInt i = 0; i < nByte; ++i)
    {
        c = ifs.get();

        if (c == EOF)
            return TS_FALSE;

        if (c <= 8 && c>1)
        {
            return TS_TRUE;
        }
    }

    return TS_FALSE;
}

TsBool TsStlLoader::CreateCommonData()
{
    m_pMeshs = TsNew(TsCommon3DMesh);

    TSUT::TsFilePathAnalyzer analizer( m_filename );
    analizer.ReExtencion("");

    m_pMeshs[0].m_name = analizer.GetFileName();

    m_pMeshs[0].m_vertexCount = m_faceSize * 3;

    m_pMeshs[0].m_pPositions = TsNew(TsVector3[m_faceSize * 3]);
    m_pMeshs[0].m_pNormals = TsNew(TsVector3[m_faceSize * 3]);

    for (TsUint i = 0; i < m_faceSize; ++i)
    for (TsUint j = 0; j < 3; ++j)
    {
        TsUint index        = (i * 3) + ( j );      //インデックス
        TsUint invIndex     = (2 - j);              //右手系 -> 左手系の　面変換用

        TsVector3 & p = m_pMeshs[0].m_pPositions[index];
        p = m_faceList[i].pos[invIndex];
        p.x *= -1;

        TsVector3 & n = m_pMeshs[0].m_pNormals[index];
        n = m_faceList[i].normal;
        n.x *= -1;

        //! AABBの作成
        m_pMeshs[0].m_aabb.AddPoint(p);
    }

    m_pMeshs[0].m_pTransoform = TsNew(TsTransForm);

    m_pMaterials = TsNew( TsCommon3DMaterial );
    m_pMaterials[0].m_name = "material:" + analizer.GetFileName();

    m_pMeshs[0].m_pMaterialRef = &m_pMaterials[0];

    m_meshCount = 1;
    m_materialCount = 1;

    return TS_TRUE;

}