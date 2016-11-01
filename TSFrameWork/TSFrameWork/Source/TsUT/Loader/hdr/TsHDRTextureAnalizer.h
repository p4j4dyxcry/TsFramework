#pragma once

//-----------------------------------------------------------------------------
// �w�b�_����ǂݎ����
//-----------------------------------------------------------------------------
struct HDRHEaderInfo{
    TsChar	magicNumber[64];		// magic number (#?�̌�A"RADIANCE"�ɂ̂ݑΉ�)
    TsChar	format[64];				// �t�H�[�}�b�g
    TsF64	gamma;
    TsF64 	exposure;				// �I������
    TsInt 	width;					// ��
    TsInt 	height;					// ����
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