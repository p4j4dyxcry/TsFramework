//!*******************************************************
//! TsBinalizerBase.h
//!
//! バイナライザのベースクラス.
//!
//! © 2016 Yuki Tsuneyama
#pragma once


#define TS_BIN_HEADER (typeid(*this).name())

class TsBinalizer
{
public:
    static const TsInt HEADER_SIZE = 40;

    struct BinalizerHeader
    {
        TsChar m_header[HEADER_SIZE];

        TsBool operator == (const BinalizerHeader& rhs)
        {
            return strcmp(m_header, rhs.m_header) == 0;
        }
    };
    virtual BinalizerHeader GetBinaryHeader()const;

protected:

    TsBool WriteHeader( std::ofstream& ofs,const TsChar* header);
    TsBool ReadHeader( std::ifstream& ifs,const TsChar* header);
};

