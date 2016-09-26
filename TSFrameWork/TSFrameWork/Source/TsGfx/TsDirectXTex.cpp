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
	TexMetadata	rawData;
	ScratchImage image;

	//char to L char
	setlocale(LC_CTYPE, "jpn");
	TsLChar lChar[255];
	size_t sz;

	mbstowcs_s(&sz, lChar, filename, _TRUNCATE);

	HRESULT hr = LoadFromWICFile(lChar,
								  0 ,
								  &rawData ,
								  image );
	if( FAILED( hr ) )
	{
		TSUT::TsLog( "Texture Load Error \n\t %ws \n" , filename );
		return nullptr;
	}

	ID3D11ShaderResourceView* resourceView;
	// 画像からシェーダリソースView DirectXTex
	hr = CreateShaderResourceView( pDev ,
								   image.GetImages() ,
								   image.GetImageCount() ,
								   rawData ,
								   &resourceView );
	if( FAILED( hr ) )
	{
		TSUT::TsLog( "Texture Load Error \n\t %ws \n" , filename );
		return nullptr;
	}

	return resourceView;
}

