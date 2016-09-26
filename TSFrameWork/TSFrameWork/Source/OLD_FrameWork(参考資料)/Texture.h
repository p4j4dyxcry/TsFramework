//!*******************************************************
//!	Texture.h
//!
//!	テクスチャの定義
//!
//!	著作権	2016 Yuki Tsuneyama
//!	作成日時	16.01.18

#pragma once

#include <d3d11.h>
#include "Common.h"
#include "DirectXTex\DirectXTex.h"

typedef std::shared_ptr<ID3D11ShaderResourceView>  TexturePtr;
typedef std::shared_ptr<ID3D11SamplerState>		   SamplerStatePtr;

class CTexture 
{
public :
	CTexture(ID3D11Device* pDevice,LPCWSTR filename);

	void Load();
	void SetSamplerDesk( D3D11_SAMPLER_DESC sampler );
	const D3D11_SAMPLER_DESC& GetSamplerDesk()const;
	TexturePtr GetResourceView()const;
	SamplerStatePtr GetSamplerState()const;
protected:
	ID3D11ShaderResourceView* CreateTexture( ID3D11Device* , LPCWCHAR lpFileName );
	ID3D11SamplerState*		  CreateSamplerState( ID3D11Device* );
	DirectX::TexMetadata		m_texData;
	DirectX::ScratchImage*		m_image;

	D3D11_SAMPLER_DESC m_samplerDesk;
	TexturePtr m_shaderResourceView;
	SamplerStatePtr m_samplerState;
};