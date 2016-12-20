#include "../../TsUT.h"

#include "TsHDRTextureAnalizer.h"

TsHDRTextureAnalizer::TsHDRTextureAnalizer():
m_textureRawData(nullptr)
{};

TsHDRTextureAnalizer::~TsHDRTextureAnalizer()
{};

TsBool TsHDRTextureAnalizer::LoadHDR( const TsString& filename )
{
    TsBinary::Load( filename );
    ReadHeader();
    ReadPixelData();

    return 1;
}

TsBool TsHDRTextureAnalizer::ReadHeader()
{
    if( m_size == 0 )
        return TS_FALSE;

    //MagicNumberのチェック
    if( m_bin[0] == '#' &&
        m_bin[1] == '?' )
    {
        auto IsSpace = []( TsChar data)
        {
            return	data == '\0' || 
                    data == '\n' ||
                    data == ' '  ||
                    data == '\t' ;
        };
        TsInt index = 0;

        //MagicNumberの読み込み
        for( ;; ++index)
        {
            //#?の後ろが空白、改行、タブがくるまでチェックする
            if( IsSpace( m_bin[index + 2] ) == TS_FALSE )
            {
                // 先頭から　#? の後を格納する
                m_header.magicNumber[index] = m_bin[index + 2];
            }
            else//終端コード
                break;
        }

        //ヘッダの読み込み（width ,height は正確にはheaderではないが・・・)
        index += 2;
        for( ;m_bin[index] != '\0'; ++index )
        {
            const TsChar * buffer = (const TsChar*)&m_bin[index];

            //! sscanf の戻り値は成功時に代入した変数の数が返る。失敗時はEof
            if( sscanf_s( buffer , "-Y %d +X %d\n" , &m_header.width , &m_header.height ) == 2 )
                break;
            else if( sscanf_s( buffer , "EXPOSURE=%g" , &m_header.exposure ) == 1 ){}
            else if( sscanf_s( buffer , "GAMMA=%g" , &m_header.gamma ) == 1 ){}
            else if( sscanf_s( buffer , "FORMAT=%s\n" , m_header.format , ( unsigned )sizeof( m_header.format ) ) == 1 ){}
        };
    }
    else
    {
        TsDebugLogError("HDRフォーマットエラー\n");
        return TS_FALSE;
    }

    return TS_TRUE;
}

TsBool TsHDRTextureAnalizer::ReadPixelData()
{
    TsInt scanlineW	= m_header.width;
    TsInt numline = m_header.width;

    TsArrayType4<TsByte> color;

    //領域確保
    m_textureRawData = TsNew(TsArrayType4<TsByte>[m_header.width * m_header.height]);
    TsArrayType4<TsByte>* rawPtr = &m_textureRawData[0];
    //ポインタをデータの先頭までシーク
    TsInt index = 0;
    SeekString("+X",0,&index);
    SeekString("\n", index,&index );
    TsByte* ptr = &m_bin[index];

    while (*ptr == '\n')
        ptr++;

    //古いバージョンは圧縮なし
    if (scanlineW < 8 || scanlineW > 0x7fff)
    {
        memcpy(m_textureRawData, ptr, (scanlineW * numline) * 4);
        return TS_TRUE;
    }

    TsByte* buffer = nullptr;
    buffer = TsNew(TsByte[scanlineW * 4]);
    while (numline > 0)
    {
        for (int i = 0; i < 4; ++i)
            color[i] = *(ptr++);
        //無圧縮
        if ((color[0] != 2) || (color[1] != 2) || (color[2] & 0x80))
        {
            m_textureRawData[0] = color;

            auto p = m_textureRawData + 3;

            memcpy(p, ptr, (scanlineW * numline) - 1);
            return TS_TRUE;
        }
        
        memset(buffer, 0, scanlineW * 4);
        TsByte* bufferPtr = &buffer[0];
        TsByte* bufferEnd;
        for (TsInt i = 0; i < 4; ++i)
        {
            bufferEnd = &buffer[(i + 1)*scanlineW];

            while (bufferPtr < bufferEnd)
            {
                TsUint c = *(ptr++);

                //圧縮判定
                if (c > 128)
                {
                    c -= 128;
                    TsByte data = *(ptr++);
                    while (c-- > 0)
                    {
                        *(bufferPtr++) = data;
                    }
                }
                else
                {
                    while (c-- > 0)
                    {
                        *(bufferPtr++) = *(ptr++);
                    }
                }
            }
        }

        for (TsInt i = 0; i<scanlineW; i++) 
        {
            (*rawPtr).x = buffer[i];
            (*rawPtr).y = buffer[i + scanlineW];
            (*rawPtr).z = buffer[i + 2 * scanlineW];
            (*rawPtr).w = buffer[i + 3 * scanlineW];
            rawPtr++;
        }
        numline--;
    }

    TsDelete( buffer);

    return TS_TRUE;
}