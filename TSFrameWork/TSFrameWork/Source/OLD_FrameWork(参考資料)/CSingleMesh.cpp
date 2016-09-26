#include "Mesh.h"

CSingleMesh::~CSingleMesh()
{
	SafeDeleteArray( m_meshVertex );
	SafeDeleteArray( m_indexList );
	SafeRelease( m_vertexBuffer );
	SafeRelease( m_indexBuffer );
}

void CSingleMesh::SetVertexData( std::vector<CMeshVertex>& vertexList )
{
	SafeDelete( m_meshVertex );
	m_meshVertexNum = vertexList.size();
	m_meshVertex = new CMeshVertex[m_meshVertexNum];

	memcpy( &m_meshVertex[0] , &vertexList[0] , sizeof( CMeshVertex ) * m_meshVertexNum );
}

void CSingleMesh::SetIndexData( std::vector<int>& indexList )
{
	SafeDelete( m_indexList );

	m_indexNum = indexList.size();
	m_indexList = new int[m_indexNum];

	memcpy( &m_indexList[0] , &indexList[0] , sizeof( int ) * indexList.size() );
}

CMeshVertex* CSingleMesh::GetMeshVertex()const
{
	return m_meshVertex;
}
int	CSingleMesh::GetMeshVertexCount()const
{
	return m_meshVertexNum;
}

UINT* CSingleMesh::GetIndexList()const
{
	return ( UINT* )m_indexList;
}
int CSingleMesh::GetIndexCount()const
{
	return m_indexNum;
}

ID3D11Buffer* CSingleMesh::GetVertexBuffer()const
{
	return m_vertexBuffer;
}

ID3D11Buffer* CSingleMesh::GetIndexBuffer()const
{
	return m_indexBuffer;
}
void CSingleMesh::CreateBuffer( const CDirectXGraphics& dx11 )
{
	//���_�o�b�t�@���쐬����
	dx11.CreateBuffer( &m_vertexBuffer , 
					   GetMeshVertex() , 
					   sizeof( CMeshVertex )*GetMeshVertexCount() ,
					   0 , 
					   D3D11_BIND_VERTEX_BUFFER );

	// �C���f�b�N�X�o�b�t�@���쐬����B
	dx11.CreateBuffer( &m_indexBuffer , 
					   GetIndexList() , 
					   sizeof( UINT )* GetIndexCount() , 
					   0 , 
					   D3D11_BIND_INDEX_BUFFER );
}

void CSingleMesh::CreateTangent()
{
	std::vector<CMeshVertex> vertexList;
	std::vector<int> indexList;
	for( int i = 0; i < m_indexNum; i ++ )
	{
		if( i % 3 != 2  )
			continue;

		int i0 = m_indexList[i-2];
		int i1 = m_indexList[i-1];
		int i2 = m_indexList[i];

		CMeshVertex v[3];
		v[0] = m_meshVertex[i0];
		v[1] = m_meshVertex[i1];
		v[2] = m_meshVertex[i2];

		Vector3 cp0[] =
		{
			Vector3( m_meshVertex[i0].m_pos.x , m_meshVertex[i0].m_texture.x , m_meshVertex[i0].m_texture.y ) ,
			Vector3( m_meshVertex[i0].m_pos.y , m_meshVertex[i0].m_texture.x , m_meshVertex[i0].m_texture.y ) ,
			Vector3( m_meshVertex[i0].m_pos.z , m_meshVertex[i0].m_texture.x , m_meshVertex[i0].m_texture.y ) 
		};

		Vector3 cp1[3] =
		{
			Vector3( m_meshVertex[i1].m_pos.x , m_meshVertex[i1].m_texture.x , m_meshVertex[i1].m_texture.y ) ,
			Vector3( m_meshVertex[i1].m_pos.y , m_meshVertex[i1].m_texture.x , m_meshVertex[i1].m_texture.y ) ,
			Vector3( m_meshVertex[i1].m_pos.z , m_meshVertex[i1].m_texture.x , m_meshVertex[i1].m_texture.y )
		};

		Vector3 cp2[3] =
		{
			Vector3( m_meshVertex[i2].m_pos.x , m_meshVertex[i2].m_texture.x , m_meshVertex[i2].m_texture.y ) ,
			Vector3( m_meshVertex[i2].m_pos.y , m_meshVertex[i2].m_texture.x , m_meshVertex[i2].m_texture.y ) ,
			Vector3( m_meshVertex[i2].m_pos.z , m_meshVertex[i2].m_texture.x , m_meshVertex[i2].m_texture.y )
		};
		// ���ʃp�����[�^����UV�����W�Z�o
		float U[3];
		for( int j = 0; j < 3; ++j ) {
			Vector3 V1 = cp1[j] - cp0[j];
			Vector3 V2 = cp2[j] - cp1[j];
			Vector3 ABC = Vector3::Closs( V1 , V2 );

			if( ABC.x == 0 )
			{
				ABC.x = 1;
			}

			U[j] = -ABC.y / ABC.x;
		}
		Vector3 tangent = Vector3( U[0] , U[1] , U[2] );
		tangent.Normalize();

		v[0].m_tangent = tangent;
		v[1].m_tangent = tangent;
		v[2].m_tangent = tangent;

		vertexList.push_back( v[0] );
		vertexList.push_back( v[1] );
		vertexList.push_back( v[2] );
	}

	std::vector<CMeshVertex> finalVertexList;
	std::vector<int> finalIndexList;
	for( unsigned int i = 0; i < vertexList.size(); i++ )
	{
		CMeshVertex& vertex = vertexList[i];
		// �d�����Ă��邩�H
		auto it = std::find( finalVertexList.begin() , finalVertexList.end() , vertex );
		if( it == finalVertexList.end() )
		{
			// �d�����Ă��Ȃ�
			finalIndexList.push_back( finalVertexList.size() );	// ���_�C���f�b�N�X���i�[
			finalVertexList.push_back( vertex );					// ���_�����i�[
		}
		else
		{
			// �d�����Ă���
			auto index = std::distance( finalVertexList.begin() , it );	// �擪���猻�C�e���[�^�i�d�����_�����擪�f�[�^���w�������Ă���j�܂ł̃C���f�b�N�X�ԍ����擾
			finalIndexList.push_back( index );								// �C���f�b�N�X�ԍ��i�d�����_�����擪�f�[�^���w�������Ă���j���C���f�b�N�X���X�g�ɃZ�b�g
		}
	}
	SetVertexData( finalVertexList );
	SetIndexData( finalIndexList );
}