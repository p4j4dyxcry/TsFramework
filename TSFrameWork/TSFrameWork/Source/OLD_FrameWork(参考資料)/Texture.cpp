#include "Texture.h"
#include "CDirectxGraphics.h"
#include "DebugConsole.h"
#if (defined(_DEBUG) | defined(DEBUG))
#pragma comment(lib,"Lib/Debug/DirectXTex.lib")
#else
#pragma comment(lib,"Lib/Release/DirectXTex.lib")
#endif

using namespace DirectX;
using namespace std;

CTexture::CTexture( ID3D11Device* pDev , LPCWSTR filename )
	: m_shaderResourceView( CreateTexture( pDev , filename ) , D3DX11_DELETER() )
	, m_samplerState(CreateSamplerState(pDev),D3DX11_DELETER())
{


}

TexturePtr CTexture::GetResourceView()const
{
	return m_shaderResourceView;
}

SamplerStatePtr CTexture::GetSamplerState()const
{
	return m_samplerState;
}

const D3D11_SAMPLER_DESC& CTexture::GetSamplerDesk()const
{
	return m_samplerDesk;
}

void CTexture::SetSamplerDesk( D3D11_SAMPLER_DESC sampler )
{
	m_samplerDesk = sampler;
}

ID3D11ShaderResourceView* CTexture::CreateTexture( ID3D11Device* pDev , LPCWSTR lpFileName )
{

	//テクスチャ作成
	// 画像ファイル読み込み DirectXTex
	TexMetadata	rawData;
	ScratchImage	image;
	HRESULT hr = LoadFromWICFile( lpFileName ,
								  0 ,
								  &rawData ,
								  image );
	m_texData = rawData;
	m_image = &image;

	if( FAILED( hr ) )
	{
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
		return nullptr;
	}

	return resourceView;

}

ID3D11SamplerState* CTexture::CreateSamplerState( ID3D11Device* pDev )
{
	// SamplerState作成
	ZeroMemory( &m_samplerDesk , sizeof( m_samplerDesk ) );
	m_samplerDesk.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	m_samplerDesk.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	m_samplerDesk.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	m_samplerDesk.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_samplerDesk.ComparisonFunc = D3D11_COMPARISON_NEVER;
	m_samplerDesk.MinLOD = 0;
	m_samplerDesk.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* sampler;
	HRESULT hr = pDev->CreateSamplerState( &m_samplerDesk , &sampler );
	if( FAILED( hr ) ){
		return nullptr;
	}

	return sampler;
}