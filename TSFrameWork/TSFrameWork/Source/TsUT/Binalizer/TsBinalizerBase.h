//!*******************************************************
//! TsBinalizerBase.h
//!
//! バイナライザのベースクラス.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsBinalizer
{
public:
    static const TsInt HEADER_SIZE = 40;
protected:
    //----------------------------------------------------------
    // peropery
    //----------------------------------------------------------

    TsBool WriteHeader( std::ofstream& ofs,const TsChar* header);

    TsBool ReadHeader( std::ifstream& ifs,const TsChar* header);


};