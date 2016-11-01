#pragma once

//-----------------------------------------------------------------------------
// ヘッダから読み取る情報
//-----------------------------------------------------------------------------
struct HDRHEaderInfo{
    TsChar	magicNumber[64];		// magic number (#?の後、"RADIANCE"にのみ対応)
    TsChar	format[64];				// フォーマット
    TsF64	gamma;
    TsF64 	exposure;				// 露光時間
    TsInt 	width;					// 幅
    TsInt 	height;					// 高さ
    //Constructor
    HDRHEaderInfo()
    {
        memset( magicNumber , 0 , 64 );
        memset( format , 0 , 64 );

        gamma =
            exposure = 1.0;

        width =
            height = 0;
    }
};

class TsHDRTextureAnalizer : TsBinary
{
public:
    TsHDRTextureAnalizer();
    virtual ~TsHDRTextureAnalizer();
    TsBool LoadHDR( const TsString& filename );
protected:
    TsBool ReadHeader();
    TsBool ReadPixelData();
    TsBool ReadPixelRawData();
    TsBool ReadRunLengthRawData();
    HDRHEaderInfo m_header;

    TsArrayType4<TsByte>* m_textureRawData;
};