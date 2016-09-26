#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

#include "DebugConsole.h"

template<typename T>
void CShader<T>::CompileFromFile(
	__in ID3D11Device* pDev ,
	__in LPCWSTR filename ,
	__in CHAR* entryPoint ,
	__in CHAR* pProfile )
{
	CHAR Profile[256];

	ID3D10Blob* pBlob = nullptr;
	ID3D10Blob* error = nullptr;

	// 行列を列優先で設定し、古い形式の記述を許可しないようにする
	UINT Flag1 = D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS;
	// 最適化レベルを設定する
#if defined(DEBUG) || defined(_DEBUG)
	Flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL0;
#else
	Flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif

	strcpy_s( Profile , pProfile );
//	ID3DInclude inc;

	// ファイルを元にエフェクトをコンパイルする
	D3DCompileFromFile( filename ,
						nullptr ,
						D3D_COMPILE_STANDARD_FILE_INCLUDE ,
						entryPoint ,
						Profile ,
						Flag1 ,
						0,
						&pBlob ,
						&error );

	if( error )
	{
		DEBUG_PRINTF( "%s",(char*)error->GetBufferPointer() );
	}

	m_resourcesSize = pBlob->GetBufferSize();
	m_resources = new BYTE[m_resourcesSize];
	
	CopyMemory( m_resources , pBlob->GetBufferPointer() , m_resourcesSize );
	SafeRelease( pBlob );

	CompileFromMemory( pDev , m_resources , m_resourcesSize );
};

template<typename T>
void CShader<T>::CompileFromCSO( ID3D11Device* pd3dDevice , TCHAR* filename )
{
	FILE* fp = _tfopen( csoName , _T( "rb" ) );

	fseek( fp , 0 , SEEK_END );
	long cso_sz = ftell( fp );
	fseek( fp , 0 , SEEK_SET );
	m_resourcesSize = cso_sz;
	m_resources = new unsigned char[cso_sz];
	fread( cso_data , cso_sz , 1 , fp );
	fclose( fp );

	CompileFromMemory( pDev , m_resources , m_resourcesSize );
}

template<>
void VertexShader::CompileFromMemory( ID3D11Device* pDev , void* memory , size_t sz )
{
	pDev->CreateVertexShader( memory , sz , nullptr , &m_shader );
	CreateInputLayout( pDev );
}

template<>
void PixelShader::CompileFromMemory( ID3D11Device* pDev , void* memory , size_t sz )
{
	pDev->CreatePixelShader( memory , sz , nullptr , &m_shader );
}
template<>
void HullShader::CompileFromMemory( ID3D11Device* pDev , void* memory , size_t sz )
{
	pDev->CreateHullShader( memory , sz , nullptr , &m_shader );
}
template<>
void GeometryShader::CompileFromMemory( ID3D11Device* pDev , void* memory , size_t sz )
{
	pDev->CreateGeometryShader( memory , sz , nullptr , &m_shader );
}
template<>
void ComputeShader::CompileFromMemory( ID3D11Device* pDev , void* memory , size_t sz )
{
	pDev->CreateComputeShader( memory , sz , nullptr , &m_shader );
}
template<>
void DomainShader::CompileFromMemory( ID3D11Device* pDev , void* memory , size_t sz )
{
	pDev->CreateDomainShader( memory , sz , nullptr , &m_shader );
}

template<>
void VertexShader::CreateInputLayout( ID3D11Device* pDev )
{
	ID3D11ShaderReflection* pVertexShaderReflection = nullptr;
	if( FAILED( D3DReflect( m_resources ,
		m_resourcesSize ,
		IID_ID3D11ShaderReflection ,
		( void** )&pVertexShaderReflection ) ) )
	{
		//input Layout 作成失敗
		return;
	}

	D3D11_SHADER_DESC shaderDesc;
	pVertexShaderReflection->GetDesc( &shaderDesc );

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for( UINT i = 0; i< shaderDesc.InputParameters; i++ )
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pVertexShaderReflection->GetInputParameterDesc( i , &paramDesc );

		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		// DXGI format
		if( paramDesc.Mask == 1 )
		{
			if( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 )
				elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 )
				elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 )
				elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if( paramDesc.Mask <= 3 )
		{
			if( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if( paramDesc.Mask <= 7 )
		{
			if( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if( paramDesc.Mask <= 15 )
		{
			if( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		inputLayoutDesc.push_back( elementDesc );
	}

	HRESULT hr = pDev->CreateInputLayout( &inputLayoutDesc[0] ,
										  inputLayoutDesc.size() ,
										  m_resources ,
										  m_resourcesSize ,
										  &m_inputLayout );

	pVertexShaderReflection->Release();
}

