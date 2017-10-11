#include "TsImGui.h"
#include "Extern\ImGui\include\imgui.h"
void TsImGui::Initialize( TsDevice* pDevice )
{

}

void TsImGui::Render( TsDeviceContext * pDeviceContext )
{

}

void TsImGui::InitializeShader( TsDevice * pDevice )
{
	m_pShaderEffect = new TsShaderEffect();

	TsVertexShader * pVertexShadder = new TsVertexShader();

	// Create the vertex shader
    {
        static const char* vertexShader =
            "cbuffer vertexBuffer : register(b0) \
            {\
            float4x4 ProjectionMatrix; \
            };\
            struct VS_INPUT\
            {\
            float2 pos : POSITION;\
            float4 col : COLOR0;\
            float2 uv  : TEXCOORD0;\
            };\
            \
            struct PS_INPUT\
            {\
            float4 pos : SV_POSITION;\
            float4 col : COLOR0;\
            float2 uv  : TEXCOORD0;\
            };\
            \
            PS_INPUT main(VS_INPUT input)\
            {\
            PS_INPUT output;\
            output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
            output.col = input.col;\
            output.uv  = input.uv;\
            return output;\
            }";

		ID3D11VertexShader* pD3DVertexShader = nullptr;
		ID3D10Blob *pBlob = nullptr;
		ID3D11InputLayout * pInputLayout = nullptr;

		D3DCompile( vertexShader , strlen( vertexShader ) , NULL , NULL , NULL , "main" , "vs_4_0" , 0 , 0 , &pBlob , NULL );

		assert( pBlob );

		if( pDevice->GetDevD3D()->CreateVertexShader( ( DWORD* )pBlob->GetBufferPointer() , pBlob->GetBufferSize() , NULL , &pD3DVertexShader ) != S_OK )
			return;

        // Create the input layout
        D3D11_INPUT_ELEMENT_DESC local_layout[] = 
		{
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (size_t)(&((ImDrawVert*)0)->pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (size_t)(&((ImDrawVert*)0)->uv),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, (size_t)(&((ImDrawVert*)0)->col), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
		if( pDevice->GetDevD3D()->CreateInputLayout( local_layout , 3 , pBlob->GetBufferPointer() , pBlob->GetBufferSize() , &pInputLayout ) != S_OK )
			return;

        // Create the constant buffer
        {
			//pDevice->CreateCBuffer()
			//pDevice->GetDevD3D()->CreateBuffer( &desc , NULL , &g_pVertexConstantBuffer );
        }
    }

}