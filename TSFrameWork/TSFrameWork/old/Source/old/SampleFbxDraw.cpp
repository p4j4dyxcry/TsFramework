//!*******************************************************
//!
//! ※本設計に入る前のテストコード
//!
//!*******************************************************
#include "SampleMesh.h"
#include "CDirectxGraphics.h"

#include "CMatrix.h"
#include <tchar.h>
#include "Fbx.h"
#include "Texture.h"
#include "Shader.h"
#include "DebugMath.h"
#include "CSkinAnimation.h"

extern CDirectXGraphics	g_pD3D11;

std::shared_ptr<CTexture> g_texture;
std::shared_ptr<CTexture> g_normal;
std::vector<CSingleTransformMesh> g_mesh;
CSkinAnimation g_skinInfo;

VertexShader vs;
PixelShader ps;

void SetCBuffer( ID3D11DeviceContext * context,
				 void * p , 
				 size_t sz ,
				 int index)
{
	ID3D11Buffer* Buffers = nullptr;

	g_pD3D11.CreateBuffer( &Buffers , p , sz , D3D11_CPU_ACCESS_WRITE , D3D11_BIND_CONSTANT_BUFFER );

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	context->Map( Buffers , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &mappedResource );

	CopyMemory( mappedResource.pData , p , sizeof( sz ) );

	context->Unmap( Buffers , 0 );

	context->VSSetConstantBuffers( index , 1 , &Buffers );

	Buffers->Release();
}

void SampleLoad()
{
	CFbxLoader fbxLoader;
	fbxLoader.Load( "Idol.fbx" );
	fbxLoader.LoadAnimation( "move.fbx" );
	g_mesh = fbxLoader.GetMeshs();
	g_skinInfo = fbxLoader.GetSkinAnimes()[0];

	g_texture = std::make_shared<CTexture>( CTexture( g_pD3D11.GetDXDevice() , L"Idol_Tex.png" ) );
	g_normal = std::make_shared<CTexture>( CTexture( g_pD3D11.GetDXDevice() , L"Idol_Normal.png" ) );
	for each( auto m in g_mesh )
		m->CreateBuffer( g_pD3D11 );
	vs.CompileFromFile( g_pD3D11.GetDXDevice() , L"sample.hlsl" , "main" , "vs_4_0" );
	ps.CompileFromFile( g_pD3D11.GetDXDevice() , L"sample.hlsl" , "PS_Main" , "ps_4_0" );
}
void SampleDraw()
{
	auto context = g_pD3D11.GetImmediateContext();

	context->ClearDepthStencilView( g_pD3D11.GetDpethStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL , 1.0f , 0 );

	context->IASetInputLayout( vs.GetInputLayout() );

	context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	static float angle = 0;
	if( GetAsyncKeyState( VK_LEFT ) )
		angle -= 0.5f;

	if( GetAsyncKeyState( VK_RIGHT ) )
		angle += 0.5f;
	Vector3 cameraPos = Vector3( 0 , 55 , 300 ) * CMatrix::CreateRotate(CQuaternion::AngleAxis(Vector3::up,angle));
	// 行列設定
	Matrix proj = 
		XMMatrixPerspectiveFovLH( 
		3.1415926f / 2.0f , 
		16.0f / 9.0f , 
		1.0f , 10000.0f );
	Matrix view =
		XMMatrixLookAtLH( cameraPos.ToXMVECTOR() ,
						 Vector3::zero.ToXMVECTOR(),
						 Vector3::up.ToXMVECTOR());

	ID3D11ShaderResourceView * data = g_texture->GetResourceView().get();
	ID3D11SamplerState * sample = g_texture->GetSamplerState().get();
	context->PSSetShaderResources( 0 , 1 ,&data  );
	context->PSSetSamplers( 0 , 1 , &sample );

	data = g_normal->GetResourceView().get();
	context->PSSetShaderResources( 1 , 1 , &data );

	context->PSSetShader( ps.GetShader() , NULL , 0 );



	struct Light0
	{
		Vector4 g_lightPos;
		Vector4 g_lightColor;
	};

	struct  Matrix1
	{
		Matrix g_wvp;
		Matrix g_wv;
		Matrix g_invView;

		Matrix g_world;
		Matrix g_invWorld;
	};

	struct CameraData2
	{
		Vector4 g_camera;
	};



	// 頂点バッファ設定
	UINT stride = sizeof( CMeshVertex );
	UINT offset = 0;
	static float t = 0;

	static int f = 2;
	f += 1;
	if( f > 99 )
		f = 2;
	t += 0.007f;


	for each( auto m in g_mesh )
	{

		m->m_boneManager.SetSkinInfo( g_skinInfo , f );
		m->m_boneManager.UpdateBones( Matrix::identity );

		Matrix world;

		Matrix trans = m.GetTransform()->ToWorldMatrix();
		world *= trans;

		world *= Matrix::CreateRotate( Quaternion::AngleAxis( Vector3::up , t ) );

		world *= Matrix::CreateTranslate( 15 , -30 , 0 );

		//world *= Matrix::CreateScale( 1.0f , 1.0f , 1.0f );

		Matrix wvp = ( world * view * proj );

		context->VSSetShader( vs.GetShader() , NULL , 0 );
	
		Light0 c0;
		c0.g_lightColor = Vector4( 1 , 0 , 0 , 1 );
		c0.g_lightPos	= Vector4( 0 , 0 , -100 , 1 );

		Matrix1 c1;
		c1.g_wvp = wvp.Transposed();
		c1.g_world = world.Transposed();
		c1.g_wv = ( world * view ).Transposed();
		c1.g_invView = view.Transposed().Inversed();
		c1.g_invWorld = world.Transpose().Inversed();	

		CameraData2 c2;
		c2.g_camera = cameraPos;

		
		//SetCBuffer( context , &c0 , sizeof( Light0 ) , 0 );
		//SetCBuffer( context , &c1 , sizeof( Matrix1 ) , 1 );
		//SetCBuffer( context , &c2 , sizeof( CameraData2 ) , 2 );

		ID3D11Buffer* Buffers = nullptr;

		g_pD3D11.CreateBuffer( &Buffers , &c0 , sizeof( c0 ) , D3D11_CPU_ACCESS_WRITE , D3D11_BIND_CONSTANT_BUFFER );
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		context->Map( Buffers , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &mappedResource );
		CopyMemory( mappedResource.pData , &c0 , sizeof( c0 ) );
		context->Unmap( Buffers , 0 );
		context->PSSetConstantBuffers( 0 , 1 , &Buffers );
		Buffers->Release();

		g_pD3D11.CreateBuffer( &Buffers , &c1 , sizeof(c1) , D3D11_CPU_ACCESS_WRITE , D3D11_BIND_CONSTANT_BUFFER );
		context->Map( Buffers , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &mappedResource );
		CopyMemory( mappedResource.pData , &c1 , sizeof( c1 ) );
		context->Unmap( Buffers , 0 );
		context->VSSetConstantBuffers( 1 , 1 , &Buffers );
		context->PSSetConstantBuffers( 1 , 1 , &Buffers );
		Buffers->Release();
		
		g_pD3D11.CreateBuffer( &Buffers , &c2 , sizeof( c2 ) , D3D11_CPU_ACCESS_WRITE , D3D11_BIND_CONSTANT_BUFFER );
		context->Map( Buffers , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &mappedResource );
		CopyMemory( mappedResource.pData , &c2 , sizeof( c2 ) );
		context->Unmap( Buffers , 0 );
		context->PSSetConstantBuffers( 2 , 1 , &Buffers );
		Buffers->Release();

		g_pD3D11.CreateBuffer( &Buffers , m->m_boneManager.m_boneTransFormMatrix , sizeof( m->m_boneManager.m_boneTransFormMatrix ) , D3D11_CPU_ACCESS_WRITE , D3D11_BIND_CONSTANT_BUFFER );
		mappedResource;
		context->Map( Buffers , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &mappedResource );
		CopyMemory( mappedResource.pData , m->m_boneManager.m_boneTransFormMatrix , sizeof( m->m_boneManager.m_boneTransFormMatrix ) );
		context->Unmap( Buffers , 0 );
		context->VSSetConstantBuffers( 3 , 1 , &Buffers );
		Buffers->Release();

		auto vertexBuffer = m->GetVertexBuffer();
		context->IASetVertexBuffers( 0 , 1 , &vertexBuffer , &stride , &offset );
		context->IASetIndexBuffer( m->GetIndexBuffer() , DXGI_FORMAT_R32_UINT , 0 );

//		context->Draw( m->GetMeshVertexCount() , 0 );
		context->DrawIndexed( m->GetIndexCount() , 0 , 0 );
	}
}

void SampleExit()
{
	for each ( auto p in g_mesh )
		p.Clear();
}