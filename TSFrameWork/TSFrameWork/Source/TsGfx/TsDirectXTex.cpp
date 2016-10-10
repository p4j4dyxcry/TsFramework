#include "../../Extern/DirectXTex/DirectXTex.h"
#include "TsGfx.h"

#ifdef _DEBUG
    #pragma comment(lib,"Lib\\Debug\\DirectXTex.lib")
#else
    #pragma comment(lib,"Lib\\Release\\DirectXTex.lib")
#endif
using namespace DirectX;

ID3D11ShaderResourceView* TsDirectXTex::LoadFromFile( ID3D11Device* pDev ,const TsChar* filename )
{
    TSUT::TsFilePathAnalyzer analize = filename;

    ScratchImage image;

    //char to L char
    setlocale(LC_CTYPE, "jpn");
    TsLChar lChar[255];
    size_t sz;

    mbstowcs_s(&sz, lChar, filename, _TRUNCATE);
    HRESULT hr;
    ID3D11ShaderResourceView* resourceView;
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
        TSUT::TsLog( "Texture Load Error \n\t %ws \n" , filename );
        return nullptr;
    }

    // 画像からシェーダリソースView DirectXTex
    hr = CreateShaderResourceView( pDev ,
                                   image.GetImages() ,
                                   image.GetImageCount() ,
                                   image.GetMetadata() ,
                                   &resourceView );
    if( FAILED( hr ) )
    {
        TSUT::TsLog( "Texture Load Error \n\t %ws \n" , filename );
        return nullptr;
    }

    return resourceView;
}

