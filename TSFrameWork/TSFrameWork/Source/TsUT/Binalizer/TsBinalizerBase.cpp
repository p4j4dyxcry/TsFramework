#include "../../../TsAfx.h"
#include "TsBinalizerBase.h"

#include <fstream>

TsBinalizer::BinalizerHeader TsBinalizer::GetBinaryHeader()const
{
    BinalizerHeader result = { 0 };
    strcpy_s(result.m_header, TS_BIN_HEADER);
    return result;
}

TsBool TsBinalizer::WriteHeader(std::ofstream& ofs, const TsChar* header)
{
    TsInt len = strlen(header);
    if (len > HEADER_SIZE - 1)
    {
        TsDebugLogError("Binalize���s \"%s\" ��%d�����𒴂��Ă��܂��B\n", header, HEADER_SIZE - 1);
        return TS_FALSE;
    }

    TsChar headerData[HEADER_SIZE] = "";

    strcpy_s(headerData, HEADER_SIZE, header);

    ofs.write(headerData, HEADER_SIZE);

    return TS_TRUE;
}

TsBool TsBinalizer::ReadHeader(std::ifstream& ifs, const TsChar* header)
{
    TsInt len = strlen(header);
    if (len > HEADER_SIZE - 1)
    {
        TsDebugLogError("Decode���s \"%s\" ��%d�����𒴂��Ă��܂��B\n", header, HEADER_SIZE - 1);
        return TS_FALSE;
    }

    TsChar headerData[HEADER_SIZE] = "";

    ifs.read(headerData, HEADER_SIZE);
    if (strcmp(headerData, header))
    {
        TsDebugLogError("Decode���s�w�b�_\"%s\"��ǂݍ��߂܂���ł����B\n", header);
        return TS_FALSE;
    }
    return TS_TRUE;
}