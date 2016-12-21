#include "../../../Extern/DirectXTex/DirectXTex.h"
#include "../TsGfx.h"

#ifdef _DEBUG
    #pragma comment(lib,"Lib\\Debug\\DirectXTex.lib")
#else
    #pragma comment(lib,"Lib\\Release\\DirectXTex.lib")
#endif
using namespace DirectX;

TsDirectXTex::Result TsDirectXTex::LoadFromFile( ID3D11Device* pDev , const TsChar* filename )
{
    Result result;
    memset( &result , 0 , sizeof( result ) );
    TSUT::TsFilePathAnalyzer analize = filename;

    ScratchImage image;

    //char to L char
    setlocale(LC_CTYPE, "jpn");
    TsLChar lChar[255];
    size_t sz;

    mbstowcs_s(&sz, lChar, filename, _TRUNCATE);
    HRESULT hr;
    if( analize.GetExtension() == ".tga" )
    {
        hr = LoadFromTGAFile( lChar ,0 ,image );
    }
    else if( analize.GetExtension() == ".dds" )
    {
        hr = LoadFromDDSFile( lChar , DDS_FLAGS_NONE , nullptr , image );
    }
    else
    {
        hr = LoadFromWICFile( lChar ,0 ,nullptr ,image );
    }
    if( FAILED( hr ) )
    {
        TsDebugLogError("Load Error \n\t \"file = %s\" \n", filename);
        return result;
    }

    // 画像からシェーダリソースView DirectXTex
    hr = CreateShaderResourceView( pDev ,
                                   image.GetImages() ,
                                   image.GetImageCount() ,
                                   image.GetMetadata() ,
                                   &result.pSrv );
    if( FAILED( hr ) )
    {
        TsDebugLogError("Load Error \n\t \"file = %s\" \n", filename);
        return result;
    }
    TsDebugLog("Load Texture \n\t \"file = %s\" \n", filename);
    result.IsAlphaEnable = image.IsAlphaAllOpaque();

    return result;
}

