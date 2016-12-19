#include "../../../../TsAfx.h"
#include <fstream>
TsStlLoader::TsStlLoader(){}
TsStlLoader:: ~TsStlLoader(){}

TsBool TsStlLoader::LoadFile(const TsChar* filename)
{
    TsBool result = TS_FALSE;

    if (IsBinary(filename))
        return LoadFromBinary(filename);
    else
        return  LoadFromAscii(filename);
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
    ofs.write((TsChar*)&m_size, sizeof(UINT));
    ofs.write((TsChar*)m_faceList, sizeof(TsStlFace) * m_size);

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

    ifs.read((TsChar*)&m_size, sizeof(TsUint));

    if (m_size > 0)
    {
        TsSafeDelete(m_faceList);
        m_faceList = TsNew(TsStlFace[m_size]);
    }
    else
    {
        return TS_FALSE;
    }

    ifs.read((TsChar*)m_faceList, sizeof(TsStlFace) * m_size);

    return true;
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
    for (TsUint i = 0; i < m_size; ++i)
    {
        TsStlFace& f = m_faceList[i];

        ofs << "facet normal" << " "
            << f.normal.x << " "
            << f.normal.y << " "
            << f.normal.z << std::endl;

        ofs << "outer loop" << std::endl;

        for (int i = 0; i < 3; ++i)
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
    auto ReadVector3 = [](TsString& source ,const char * element, TsVector3& v )
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
    m_size = faceList.size();
    if (m_size == 0)
    {
        TsSafeDelete(m_faceList);
        return TS_FALSE;
    }

    TsSafeDelete(m_faceList);
    m_faceList = TsNew(TsStlFace[m_size]);
    memcpy(m_faceList, &faceList[0], sizeof(TsStlFace) * m_size);

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

    int 	c;
    // nバイト探索しバイナリコードが見つかればバイナリとする
    for (int i = 0; i < nByte; ++i)
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