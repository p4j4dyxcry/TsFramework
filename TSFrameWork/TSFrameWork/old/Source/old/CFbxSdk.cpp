#include <Windows.h>
#include "CFbxSdk.h"
#include "CSkinAnimation.h"
#include "DebugMath.h"

using namespace std;

CMatrix FbxMtxToCMtx( const FbxMatrix& mtx )
{
	CMatrix m;

	for( int i = 0; i < 16; ++i )
		m.m[0][i] = static_cast<float>(mtx[0][i]);

	return m;
}

//  �m�[�h���X�P���g�����H
bool    IsFbxNodeAttributeSkelton( const FbxNode* pNode )
{
	if( pNode == nullptr ) 
		return false;

	const FbxNodeAttribute* pAttr = pNode->GetNodeAttribute();

	if( pAttr == nullptr ) 
		return false;

	return  pAttr->GetAttributeType() == FbxNodeAttribute::eSkeleton;
}

//! �R���X�g���N�^
CFbxLoader::CFbxLoader(): 
	m_pFbxMgr(nullptr),
	m_pFbxImp(nullptr),
	m_pFbxScene(nullptr)
{
	m_pFbxMgr = FbxManager::Create();
	m_pFbxImp = FbxImporter::Create( m_pFbxMgr , "imp" );
	m_pFbxScene = FbxScene::Create( m_pFbxMgr , "scene" );
}

//! �f�X�g���N�^
CFbxLoader::~CFbxLoader()
{
	//! fbx sdk �̎d�l�̂���Manager���J�����邱�Ƃł��ׂĉ�������
	FbxSafeDestory( m_pFbxMgr );
}

//!*******************************************************
//!	[���\�b�h] Fbx�f�[�^�����[�h����
//!
//! ����
//!		const char*	�t�@�C���̖��O
//! �ߒl
//!		bool	true �����@: false ���s
bool CFbxLoader::Load( const char * fn )
{
	m_pFbxImp->Initialize( fn );
	m_pFbxImp->Import( m_pFbxScene );

	// �|���S�����O�p�`��
	FbxGeometryConverter geometryConverter( m_pFbxMgr );
	geometryConverter.Triangulate( m_pFbxScene , true );

	FbxNode* pNode = m_pFbxScene->GetRootNode();

	//�m�[�h�c���[�̏�����
	m_nodeTree.clear();

	// �V�[���Ɋ܂܂�郁�b�V���E�}�e���A���̉��
	int meshCount = m_pFbxScene->GetMemberCount<FbxMesh>();
	int nodeCount = m_pFbxScene->GetNodeCount();
	int materialCount = m_pFbxScene->GetMaterialCount();

	m_meshList.reserve( meshCount );
	m_materialList.reserve( materialCount );

	for( int i = 0; i < materialCount; ++i ){
		FbxSurfaceMaterial* fbxMaterial = m_pFbxScene->GetMaterial( i );
		CMaterialBase modelMaterial = ParseMaterial( fbxMaterial );
		m_materialList.push_back( modelMaterial );
		m_materialTree.insert( std::pair<std::string , int>( modelMaterial.m_materialName , i ) );
	}

	// �m�[�h������m�[�hID���擾�ł���悤�Ɏ����ɓo�^
	for( int i = 0; i < nodeCount; i++ )
	{
		FbxNode* fbxNode = m_pFbxScene->GetNode( i );
		m_nodeTree.insert( pair<string , int>( fbxNode->GetName() , i ) );

		//�{�[���K�w�\���̍쐬
		CTransForm* t = new CTransForm(FbxMtxToCMtx( fbxNode->EvaluateLocalTransform()) );
		t->m_name = fbxNode->GetName();

		m_boneMatrixTree.insert( pair<string , CTransForm*>( fbxNode->GetName() ,t  ));

		//���b�V���p
		t = new CTransForm( FbxMtxToCMtx( fbxNode->EvaluateLocalTransform() ) );
		t->m_name = fbxNode->GetName();

		m_meshMatrixTree.insert( pair<string , CTransForm*>( fbxNode->GetName() , t ) );
	}

	for( auto it = m_nodeTree.begin(); it != m_nodeTree.end(); ++it )
	{
		FbxNode* node = m_pFbxScene->GetNode( it->second );

		while( node->GetParent() != nullptr )
		{
			m_boneMatrixTree[node->GetName()]->SetParent( m_boneMatrixTree[node->GetParent()->GetName()] );
			m_meshMatrixTree[node->GetName()]->SetParent( m_meshMatrixTree[node->GetParent()->GetName()] );
			node = node->GetParent();
		}
	}

	for( int i = 0; i < meshCount; ++i ){
		FbxMesh* fbxMesh = m_pFbxScene->GetMember<FbxMesh>( i );
		m_meshList.push_back( ParseMesh( fbxMesh ) );
	}
	return true;
}

CSingleTransformMesh CFbxLoader::ParseMesh( FbxMesh * mesh )
{
	FbxNode* fbxnode = mesh->GetNode();

	CSingleTransformMesh modelMesh = new CSingleMesh();

	// �����X�L���̐����擾
	int skin = mesh->GetDeformerCount( FbxDeformer::eSkin );		// �����X�L�����̐����Z�b�g
	modelMesh->m_name = fbxnode->GetName();			
	CMaterialBase material = m_materialList[m_materialTree[fbxnode->GetMaterial( 0 )->GetName()]];
	modelMesh->m_material = material;

	//���f���ɍs���ݒ�
	modelMesh.SetTransform( m_boneMatrixTree[fbxnode->GetName()] );

	// �x�[�X�|�[�Y�̋t�s����쐬���Ă���(���b�V����)
	FbxMatrix baseposeMatrix = fbxnode->EvaluateGlobalTransform().Inverse();

	//fbx �̍s��́@double�@�Ȃ̂ŁA�@float �ň���
	double* baseposeMatrixPtr = ( double* )baseposeMatrix;
	for( int i = 0; i < 16; ++i )
	{
		modelMesh->m_inversMatrix.m[0][i] = 
			static_cast<float>( baseposeMatrixPtr[i] );
	}

	// �C���f�b�N�X�u�d�b�s�n�q�擾
	std::vector<int> indexList = GetIndexList( mesh );

	// ���_�f�[�^�擾
	std::vector<Vector3> positionList	= GetPositionList( mesh , indexList );		// ���_���W���擾�i�ʂ̍\�����ɍ��킹��j
	std::vector<Vector3> normalList		= GetNormalList( mesh , indexList );		// �@���x�N�g�����擾�i�ʂ̍\�����ɍ��킹��j
	std::vector<Vector3> tangentList	= GetTangentList( mesh , indexList );
	std::vector<Vector2> uv0List		= GetUVList( mesh , indexList );			// UV���W���擾�i�ʂ̍\�����ɍ��킹��j
	std::vector<CMeshVertex>  tmpmodelVertexList;

	std::vector<string> boneNameList;
	std::vector<Matrix> invBone;

	// �E�G�C�g�l���擾����
	std::vector<ModelBoneWeight> boneWeightList;
	GetWeight( mesh ,											// ���b�V���i���́j
			   indexList ,										// �ʂ̍\�����i���́j
			   boneWeightList ,									// �{�[����񃊃X�g�i�o�͒l�j�i�{�[�����ւ̃C���f�b�N�X�l�ƃE�F�C�g�l�j
			   boneNameList);										// �{�[���I�t�Z�b�g�s����u�d�b�s�n�q�ɕۑ��i�j

	for( UINT i = 0; i < boneNameList.size();++i )
	{
		CBone* bone = new CBone();
		bone->InitializeBone( m_boneMatrixTree[boneNameList[i]] );
		bone->m_name = boneNameList[i];
		modelMesh->m_boneManager.AddBone( bone , i );
	}

	FbxSkin* skinmesh = static_cast<FbxSkin*>( mesh->GetDeformer( 0 , FbxDeformer::eSkin ) );


	class Reciv
	{
	public:
		static void DrawBone( CBone* b , int n = 0 )
		{
			std::string str;
			for( int i = 0; i < n; ++i )
				str += '\t';
			str += b->m_name;
			str += "\n";
			DEBUG_PRINTF( str.c_str() );

			if( b->m_firstChild )
				DrawBone( b->m_firstChild ,n+1);
			if( b->m_subling )
				DrawBone( b->m_subling ,n);
		}
	};


	if( boneNameList.size() > 0 )
	{
		Reciv::DrawBone( modelMesh->m_boneManager.m_rootBone );
	}

	

	// ���_�C���f�b�N�X�����i�ʂ̍\����񏇁j���[�v����
	for( unsigned int i = 0; i < indexList.size(); i++ )
	{
		CMeshVertex vertex;
		vertex.m_pos = positionList[i];			// ���_���
		vertex.m_pos.w = 1;
		vertex.m_normal = normalList[i];				// �@�����
		if( tangentList.empty() == false )
			vertex.m_tangent = tangentList[i];

		if( uv0List.empty() == false )
			vertex.m_texture = uv0List[i];
		// �{�[���E�F�C�g�l�����݂��邩�H
		if( boneWeightList.size() > 0 )
		{
			// �{�[���̃C���f�b�N�X�ԍ����Z�b�g
			for( int j = 0; j < 4; ++j )
			{
				vertex.m_weight.m_boneIndex[j] = boneWeightList[i].m_boneIndex[j];
			}
			// �E�F�C�g�l���Z�b�g
			vertex.m_weight.m_boneWeight = boneWeightList[i].m_boneWeight;
		}

		tmpmodelVertexList.push_back( vertex );			// 
	}

	std::vector<CMeshVertex> finalVertexList;
	std::vector<int>		 finalIndexList;
	// �d�����_�������ăC���f�b�N�X�쐬
	finalVertexList.reserve( tmpmodelVertexList.size() );		// ���_��񐔕��G���A���m�ۂ���

	finalIndexList.reserve( indexList.size() );		// �ʂ̍\����񐔕��G���A���m�ۂ���

	// �����ŏd�����Ă��钸�_�f�[�^�������A���_�f�[�^���X�g�ƃC���f�b�N�X���X�g����蒼���Ă���
	// �d���������_���܂񂾒��_�������[�v���Ă���(�ʂ̍\���������ɔ����o�������_���)
	for( unsigned int i = 0; i < tmpmodelVertexList.size(); i++ )
	{
		CMeshVertex& vertex = tmpmodelVertexList[i];

		// �d�����Ă��邩�H
		// modelMesh.vertexList�́A�ŏ���ł��񂾂�o�^����Ă����i�d�����Ă��Ȃ����_���Ƃ��āj
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
	modelMesh->SetVertexData( finalVertexList );
	modelMesh->SetIndexData( finalIndexList );
	if( tangentList.empty() )
		modelMesh->CreateTangent();
	return modelMesh;
}

std::vector<Vector2> CFbxLoader::GetUVList( FbxMesh* mesh , const std::vector<int>& indexList)
{
	std::vector<Vector2> uvList;

	FbxGeometryElementUV* element = mesh->GetElementUV( 0 );					// UV�Z�b�g���擾
	FbxLayerElement::EMappingMode mappingMode = element->GetMappingMode();		// �}�b�s���O���[�h���擾
	FbxLayerElement::EReferenceMode referenceMode = element->GetReferenceMode();	// ���t�@�����X���[�h���擾	
	const FbxLayerElementArrayTemplate<int>& indexArray = element->GetIndexArray();	// �t�u�����i�[�����u�d�b�s�n�q�z��̃C���f�b�N�X�z����擾
	const FbxLayerElementArrayTemplate<FbxVector2>& directArray = element->GetDirectArray();	// �t�u�l�z����擾

	// ���_�C���f�b�N�X�����������������e�ʂ��m��
	uvList.reserve( indexList.size() );

	// ���_�ɑΉ����Ċi�[����Ă���ꍇ
	if( mappingMode == FbxGeometryElement::eByControlPoint )
	{
		// ���_���W�Ń}�b�s���O
		for( unsigned int i = 0; i < indexList.size(); i++ )
		{
			int index = indexList[i];			// �ʂ̍\�����z�񂩂璸�_�C���f�b�N�X�ԍ����擾

			// ���t�@�����X���[�h�𔻒�
			int uvIndex;
			if( referenceMode == FbxGeometryElement::eDirect ){		// eDirect�̏ꍇ
				uvIndex = index;		//�@eDirect�̏ꍇ�i���_�C���f�b�N�X�Ɠ����C���f�b�N�X�l�ŃZ�b�g����Ă���j
			}
			else{													// eIndexToDirect�̏ꍇ
				uvIndex = indexArray.GetAt( index );				// ���_���W�C���f�b�N�X�ɑΉ������t�u���C���f�b�N�X���擾
			}

			FbxVector2 uv = directArray.GetAt( uvIndex );		// uv�l��double�^�Ŏ擾
			uvList.push_back( Vector2( static_cast<float>( uv[0] ) , static_cast<float>( uv[1] ) ) );		// float�l�Ƃ��Ċi�[
		}
	}
	// �ʂ̍\�����ɑΉ����Ċi�[����Ă���ꍇ
	else if( mappingMode == FbxGeometryElement::eByPolygonVertex )
	{
		// �|���S���o�[�e�b�N�X�i�ʂ̍\�����̃C���f�b�N�X�j�Ń}�b�s���O
		int indexByPolygonVertex = 0;						// �ʂ̍\�����C���f�b�N�X�z��̃C���f�b�N�X
		int polygonCount = mesh->GetPolygonCount();			// ���b�V���̃|���S�������擾
		for( int i = 0; i < polygonCount; ++i )				// �|���S���������[�v
		{
			int polygonSize = mesh->GetPolygonSize( i );		// ���Ԗڂ̃|���S�����_�����擾

			// �|���S���̒��_�������[�v
			for( int j = 0; j < polygonSize; ++j )
			{
				// ���t�@�����X���[�h�̔���H
				int uvIndex;
				if( referenceMode == FbxGeometryElement::eDirect ){		// eDirect�̏ꍇ
					uvIndex = indexByPolygonVertex;		//�@eDirect�̏ꍇ�i���_�C���f�b�N�X�Ɠ����C���f�b�N�X�l�ŃZ�b�g����Ă���j
				}
				else{													// eIndexToDirect�̏ꍇ
					uvIndex = indexArray.GetAt( indexByPolygonVertex );	// �ʂ̍\�����C���f�b�N�X�ɑΉ������t�u���C���f�b�N�X���擾
				}
				FbxVector2 uv = directArray.GetAt( uvIndex );

				uvList.push_back( Vector2( static_cast<float>( uv[0] ) , static_cast<float>( uv[1] ) ) );	// �����������l�Ƃ��Ċi�[

				++indexByPolygonVertex;						// ���_�C���f�b�N�X���C���N�������g
			}
		}
	}
	else
	{
		DEBUG_PRINTF( "�Ή����Ă��Ȃ��@UV�i�[" );
	}

	return uvList;
}

void CFbxLoader::GetWeight( FbxMesh* mesh ,								// �e�a�w�l�d�r�g
							const std::vector<int>& indexList ,						// �ʂ̍\�����i���_�C���f�b�N�X�ԍ��j
							std::vector<ModelBoneWeight>& boneWeightList ,			// �E�F�C�g�l�i�[�u�d�b�s�n�q
							std::vector<std::string>& boneNodeNameList )
{
	// ���b�V�����̃X�L����񐔂��擾����
	int skinCount = mesh->GetDeformerCount( FbxDeformer::eSkin );
	if( skinCount == 0 )
	{
		return;
	}

	// �X�L����1�ɂ����Ή����Ȃ�

	if( skinCount > 1 )
		DEBUG_PRINTF( "�X�L�����Q�ȏ�͑Ή����܂���" );

	int controlPointsCount = mesh->GetControlPointsCount();		// ���b�V���̒��_�����擾

	std::vector<std::vector<std::pair<int , float>>> tmpBoneWeightList( controlPointsCount );	// �{�[���C���f�b�N�X�l�ƃE�F�C�g�l���y�A�ɂ����u�d�b�s�n�q

	// �e�����l����������X�L���������o��
	FbxSkin* skin = static_cast<FbxSkin*>( mesh->GetDeformer( 0 , FbxDeformer::eSkin ) );
	int clusterCount = skin->GetClusterCount();	// �X�L������N���X�^�i�{�[���j�����擾

	// �N���X�^�[�i�{�[���̎��j�̐������[�v
	for( int i = 0; i < clusterCount; ++i )
	{
		FbxCluster* cluster = skin->GetCluster( i );					// i�Ԗڂ̃N���X�^�i�{�[���j���擾

		// eNormalize�����Ή����Ȃ�
		if( cluster->GetLinkMode() != FbxCluster::eNormalize )
		{
			DEBUG_PRINTF( "%s,�Ή����Ă��Ȃ��N���X�^�`��\n" ,cluster->GetLink()->GetName());
			continue;
		}
		
		boneNodeNameList.push_back( cluster->GetLink()->GetName() );	// �{�[�������i�[
		int indexCount = cluster->GetControlPointIndicesCount();	// �{�[���ɏ������钸�_�C���f�b�N�X�����擾
		int* indices = cluster->GetControlPointIndices();			// ���_�C���f�b�N�X�z����擾
		double* weights = cluster->GetControlPointWeights();		// �E�F�C�g�l�z����擾

		// �{�[���ɏ������钸�_�C���f�b�N�X�������[�v����
		for( int j = 0; j < indexCount; ++j )
		{
			int controlPointIndex = indices[j];		// ���̃{�[���ɉe�����󂯂�j�Ԗڂ̒��_�C���f�b�N�X�ԍ����擾
			tmpBoneWeightList[controlPointIndex].push_back( std::pair<int , float>( i , static_cast<float>( weights[j] ) ) );
		}
	}

	// �R���g���[���|�C���g�ɑΉ������E�F�C�g���쐬
	std::vector<ModelBoneWeight> boneWeightListControlPoints;

	for( unsigned int j = 0; j < tmpBoneWeightList.size(); j++ ){
		// ���Ԗڂ̒��_�̃E�F�C�g�l���擾
		std::vector<std::pair<int , float>>& tmpBoneWeight = tmpBoneWeightList[j];

		// �E�F�C�g�l�̑傫���Ń\�[�g
		std::sort( tmpBoneWeight.begin() , tmpBoneWeight.end() ,
				   []( const std::pair<int , float>& weightA , const std::pair<int , float>& weightB ){ return weightA.second > weightB.second; }
		);

		// 1���_��4��葽���E�F�C�g������U���Ă���Ȃ�e�������Ȃ����͖̂�������
		while( tmpBoneWeight.size() > 4 )
		{
			tmpBoneWeight.pop_back();
		}

		// 4�ɖ����Ȃ��ꍇ�̓_�~�[��}��
		while( tmpBoneWeight.size() < 4 )
		{
			tmpBoneWeight.push_back( std::pair<int , float>( 0 , 0.0f ) );
		}

		ModelBoneWeight weight;
		float total = 0.0f;

		weight.m_boneWeight.x = tmpBoneWeight[0].second;
		weight.m_boneWeight.y = tmpBoneWeight[1].second;
		weight.m_boneWeight.z = tmpBoneWeight[2].second;
		weight.m_boneWeight.w = tmpBoneWeight[3].second;

		for( int i = 0; i < 4; ++i )
		{
			weight.m_boneIndex[i] = tmpBoneWeight[i].first;
			total += tmpBoneWeight[i].second;
		}

		// �E�F�C�g�̐��K��
		weight.m_boneWeight.x /= total;
		weight.m_boneWeight.y /= total;
		weight.m_boneWeight.z /= total;
		weight.m_boneWeight.w /= total;

		boneWeightListControlPoints.push_back( weight );
	}

	// �C���f�b�N�X�œW�J
	for( unsigned int j = 0; j < indexList.size(); j++ ){
		int index = indexList[j];				// j�Ԗڂ̒��_�C���f�b�N�X���擾
		boneWeightList.push_back( boneWeightListControlPoints[index] );	// j�Ԗڂ̒��_�̃{�[���ԍ��ƃE�G�C�g�l���Z�b�g
	}
}

std::vector<Vector3> CFbxLoader::GetNormalList( FbxMesh* mesh , const std::vector<int>& indexList ){

	int elementCount = mesh->GetElementNormalCount();			// ���̖@����񂪃Z�b�g����Ă��邩

	FbxGeometryElementNormal* element = mesh->GetElementNormal( 0 );	// �O�Ԗڂ̖@���Z�b�g���擾
	
	FbxLayerElement::EMappingMode mappingmode = element->GetMappingMode();	// �}�b�s���O���[�h�擾
	FbxLayerElement::EReferenceMode referrenceMode = element->GetReferenceMode();	// ���t�@�����X���[�h�擾
	const FbxLayerElementArrayTemplate<int>& indexArray = element->GetIndexArray();	// �@�������i�[�����������������z��̃C���f�b�N�X�l�z����擾 
	const FbxLayerElementArrayTemplate<FbxVector4>& directArray = element->GetDirectArray();	// �@�������i�[�����������������z����擾

	// �@�����u�d�b�s�n�q
	std::vector<Vector3> normalList;
	normalList.reserve( indexList.size() );		// ���_�C���f�b�N�X�����m��

	// ���_���W�Ń}�b�s���O����Ă���ꍇ
	if( mappingmode == FbxGeometryElement::eByControlPoint ){
		for( unsigned int i = 0; i < indexList.size(); i++ ){
			int index = indexList[i];		// �ʂ̍\�����z���i�Ԗڂ̒��_�C���f�b�N�X���擾
			int normalIndex;				// �@�����̃C���f�b�N�X
			// ���t�@�����X���[�h�𔻒�
			if( referrenceMode == FbxGeometryElement::eDirect ){		// eDirect�̏ꍇ
				normalIndex = index;								// ���_�C���f�b�N�X�̈ʒu�ɑΉ����ĕۑ�����Ă���
			}
			else{													// eIndexToDirect�̏ꍇ
				normalIndex = indexArray.GetAt( index );				// ���_���W�C���f�b�N�X�ɑΉ������@�����VECTOR�̃C���f�b�N�X���擾
			}
			FbxVector4 normal = directArray.GetAt( normalIndex );		// �@�������擾
			if( normal[3] != 0.0f ){
				if( normal[3] == 1.0f ){
					normalList.push_back( Vector3( static_cast<float>( normal[0] ) , static_cast<float>( normal[1] ) , static_cast<float>( normal[2] ) ) );			// �������W��
				}
				else{
					normalList.push_back( Vector3( static_cast<float>( normal[0] / normal[3] ) , static_cast<float>( normal[1] / normal[3] ) , static_cast<float>( normal[2] / normal[3] ) ) );		// �������W��
				}
			}
			else
				normalList.push_back( Vector3( static_cast<float>( normal[0] ) , static_cast<float>( normal[1] ) , static_cast<float>( normal[2] ) ) );		// �������W��
		}
	}
	else if( mappingmode == FbxGeometryElement::eByPolygonVertex ){
		// ���_�C���f�b�N�X(�ʂ̍\�����)�Ń}�b�s���O����Ă���ꍇ
		int indexByPolygonVertex = 0;
		int polygonCount = mesh->GetPolygonCount();					// �|���S�������擾
		for( int i = 0; i < polygonCount; ++i ){
			int polygonSize = mesh->GetPolygonSize( i );				// ���_�����擾
			int normalIndex;
			for( int j = 0; j < polygonSize; ++j ){
				// ���t�@�����X���[�h�𔻒�
				if( referrenceMode == FbxGeometryElement::eDirect ){		// eDirect�̏ꍇ
					normalIndex = indexByPolygonVertex;
				}
				else{
					normalIndex = indexArray.GetAt( indexByPolygonVertex );	// eIndexToDirect�̏ꍇ
				}
				FbxVector4 normal = directArray.GetAt( normalIndex );		// �@�������擾
				if( normal[3] != 0.0f ){
					if( normal[3] == 1.0 ){
						normalList.push_back( Vector3( static_cast<float>( normal[0] ) , static_cast<float>( normal[1] ) , static_cast<float>( normal[2] ) ) );		// �������W��
					}
					else{
						normalList.push_back( Vector3( static_cast<float>( normal[0] / normal[3] ) , static_cast<float>( normal[1] / normal[3] ) , static_cast<float>( normal[2] / normal[3] ) ) );	// �������W��
					}
				}
				else
					normalList.push_back( Vector3( static_cast<float>( normal[0] ) , static_cast<float>( normal[1] ) , static_cast<float>( normal[2] ) ) );		// �������W��
				++indexByPolygonVertex;
			}
		}
	}
	else{
		//todo ���Ή�
		for( int i = 0; i < (signed)indexList.size(); ++i )
			normalList.push_back( Vector3( 1 , 1 , 1 ).Normalize() );
	}

	return normalList;
}

std::vector<Vector3> CFbxLoader::GetTangentList( FbxMesh* mesh , const std::vector<int>& indexList ){

	int elementCount = mesh->GetElementTangentCount();			// ���̖@����񂪃Z�b�g����Ă��邩

	if( elementCount == 0 )
		return std::vector<Vector3>();

	FbxGeometryElementTangent* element = mesh->GetElementTangent( 0 );	// �O�Ԗڂ̖@���Z�b�g���擾

	FbxLayerElement::EMappingMode mappingmode = element->GetMappingMode();	// �}�b�s���O���[�h�擾
	FbxLayerElement::EReferenceMode referrenceMode = element->GetReferenceMode();	// ���t�@�����X���[�h�擾
	const FbxLayerElementArrayTemplate<int>& indexArray = element->GetIndexArray();	// �@�������i�[�����������������z��̃C���f�b�N�X�l�z����擾 
	const FbxLayerElementArrayTemplate<FbxVector4>& directArray = element->GetDirectArray();	// �@�������i�[�����������������z����擾

	// �@�����u�d�b�s�n�q
	std::vector<Vector3> tanList;
	tanList.reserve( indexList.size() );		// ���_�C���f�b�N�X�����m��

	// ���_���W�Ń}�b�s���O����Ă���ꍇ
	if( mappingmode == FbxGeometryElement::eByControlPoint ){
		for( unsigned int i = 0; i < indexList.size(); i++ ){
			int index = indexList[i];		// �ʂ̍\�����z���i�Ԗڂ̒��_�C���f�b�N�X���擾
			int normalIndex;				// �@�����̃C���f�b�N�X
			// ���t�@�����X���[�h�𔻒�
			if( referrenceMode == FbxGeometryElement::eDirect ){		// eDirect�̏ꍇ
				normalIndex = index;								// ���_�C���f�b�N�X�̈ʒu�ɑΉ����ĕۑ�����Ă���
			}
			else{													// eIndexToDirect�̏ꍇ
				normalIndex = indexArray.GetAt( index );				// ���_���W�C���f�b�N�X�ɑΉ������@�����VECTOR�̃C���f�b�N�X���擾
			}
			FbxVector4 normal = directArray.GetAt( normalIndex );		// �@�������擾
			if( normal[3] != 0.0f ){
				if( normal[3] == 1.0f ){
					tanList.push_back( Vector3( static_cast<float>( normal[0] ) , static_cast<float>( normal[1] ) , static_cast<float>( normal[2] ) ) );			// �������W��
				}
				else{
					tanList.push_back( Vector3( static_cast<float>( normal[0] / normal[3] ) , static_cast<float>( normal[1] / normal[3] ) , static_cast<float>( normal[2] / normal[3] ) ) );		// �������W��
				}
			}
			else
				tanList.push_back( Vector3( static_cast<float>( normal[0] ) , static_cast<float>( normal[1] ) , static_cast<float>( normal[2] ) ) );		// �������W��
		}
	}
	else if( mappingmode == FbxGeometryElement::eByPolygonVertex ){
		// ���_�C���f�b�N�X(�ʂ̍\�����)�Ń}�b�s���O����Ă���ꍇ
		int indexByPolygonVertex = 0;
		int polygonCount = mesh->GetPolygonCount();					// �|���S�������擾
		for( int i = 0; i < polygonCount; ++i ){
			int polygonSize = mesh->GetPolygonSize( i );				// ���_�����擾
			int normalIndex;
			for( int j = 0; j < polygonSize; ++j ){
				// ���t�@�����X���[�h�𔻒�
				if( referrenceMode == FbxGeometryElement::eDirect ){		// eDirect�̏ꍇ
					normalIndex = indexByPolygonVertex;
				}
				else{
					normalIndex = indexArray.GetAt( indexByPolygonVertex );	// eIndexToDirect�̏ꍇ
				}
				FbxVector4 normal = directArray.GetAt( normalIndex );		// �@�������擾
				if( normal[3] != 0.0f ){
					if( normal[3] == 1.0 ){
						tanList.push_back( Vector3( static_cast<float>( normal[0] ) , static_cast<float>( normal[1] ) , static_cast<float>( normal[2] ) ) );		// �������W��
					}
					else{
						tanList.push_back( Vector3( static_cast<float>( normal[0] / normal[3] ) , static_cast<float>( normal[1] / normal[3] ) , static_cast<float>( normal[2] / normal[3] ) ) );	// �������W��
					}
				}
				else
					tanList.push_back( Vector3( static_cast<float>( normal[0] ) , static_cast<float>( normal[1] ) , static_cast<float>( normal[2] ) ) );		// �������W��
				++indexByPolygonVertex;
			}
		}
	}
	else{
		//todo ���Ή�
		for( int i = 0; i < ( signed )indexList.size(); ++i )
			tanList.push_back( Vector3( 1 , 1 , 1 ).Normalize() );
	}

	return tanList;
}

std::vector<Vector3> CFbxLoader::GetPositionList( FbxMesh* mesh , const std::vector<int>& indexList ){
	static int b = 0;
	b++;
	//�@���_���W���擾
	std::vector<Vector3> positionList;					//	���_���W�i�[�u�d�b�s�n�q 
	positionList.reserve( indexList.size() );				//  
	for( unsigned int i = 0; i < indexList.size(); i++ ){
		int index = indexList[i];						// i�Ԗڂ̒��_���i�ʂ̍\�����񏇂ɕ���ł���j���擾
		FbxVector4 Position = mesh->GetControlPointAt( index );		// index�Ԗڂ̒��_���W���擾
		if( Position[3] != 0.0 ){
			if( Position[3] == 1.0 ){
				positionList.push_back( Vector3( static_cast<float>( Position[0] ) , static_cast<float>( Position[1] ) , static_cast<float>( Position[2] ) ) );			// �������W��
			}
			else{
				positionList.push_back( Vector3( static_cast<float>( Position[0] / Position[3] ) , static_cast<float>( Position[1] / Position[3] ) , static_cast<float>( Position[2] / Position[3] ) ) );		// �������W��
			}
		}
		else{
			positionList.push_back( Vector3( static_cast<float>( Position[0] ) , static_cast<float>( Position[1] ) , static_cast<float>( Position[2] ) ) );				// �v�𖳎����Ċi�[
//			if( b == 3 )
			{
			//	CMatrix m = FbxMtxToCMtx( mesh->GetNode()->EvaluateGlobalTransform() );
			//	DebugLog( positionList[positionList.size() - 1] * m );
//				positionList[positionList.size() - 1] *= 10;
			}
		}
	}
	return positionList;
}


std::vector<int> CFbxLoader::GetIndexList( FbxMesh* mesh )
{
	int polygonCount = mesh->GetPolygonCount();			// �O�p�`�̐����擾
	std::vector<int> indexList;							// ���_�C���f�b�N�X�i�ʂ̍\�����j
	indexList.reserve( polygonCount * 3 );				// 3�p�`�̐��~�R

	// �ʂ̍\�������擾����
	for( int i = 0; i < polygonCount; i++ ){
		indexList.push_back( mesh->GetPolygonVertex( i , 0 ) );		// i�Ԗڂ̎O�p�`�̂O�Ԗڂ̒��_�C���f�b�N�X���擾
		indexList.push_back( mesh->GetPolygonVertex( i , 1 ) );		// i�Ԗڂ̎O�p�`�̂P�Ԗڂ̒��_�C���f�b�N�X���擾
		indexList.push_back( mesh->GetPolygonVertex( i , 2 ) );		// i�Ԗڂ̎O�p�`�̂Q�Ԗڂ̒��_�C���f�b�N�X���擾
	}
	return indexList;
}

CMaterialBase CFbxLoader::ParseMaterial( FbxSurfaceMaterial* material )
{
	CMaterialBase modelMaterial;
	modelMaterial.m_materialName = material->GetName();

	auto implementation = GetImplementation( material , FBXSDK_IMPLEMENTATION_CGFX );

	if( implementation == nullptr )
	{

	}

	return CMaterialBase();

	auto rootTable = implementation->GetRootTable();
	auto entryCount = rootTable->GetEntryCount();

	for( unsigned int i = 0; i < entryCount; ++i ){
		auto entry = rootTable->GetEntry( i );
		auto fbxProperty = material->FindPropertyHierarchical( entry.GetSource() );
		if( !fbxProperty.IsValid() ){
			fbxProperty = material->RootProperty.FindHierarchical( entry.GetSource() );
		}

		auto textureCount = fbxProperty.GetSrcObjectCount<FbxTexture>();
		if( textureCount>0 ){
			std::string src = entry.GetSource();

			for( int j = 0; j < fbxProperty.GetSrcObjectCount<FbxFileTexture>(); ++j ){

				auto tex = fbxProperty.GetSrcObject<FbxFileTexture>( j );
				std::string texName = tex->GetFileName();
				texName = texName.substr( texName.find_last_of( '/' ) + 1 );

				if( src == "Maya|DiffuseTexture" ){
					modelMaterial.m_baseTex = texName;
				}
				else if( src == "Maya|NormalTexture" ){
					modelMaterial.m_normalTex = texName;
				}
				else if( src == "Maya|SpecularTexture" ){
					modelMaterial.m_specularTex = texName;
				}
				else if( src == "Maya|FalloffTexture" ){
				}
				else if( src == "Maya|ReflectionMapTexture" ){
				}
			}
		}
	}
	return modelMaterial;
}

bool CFbxLoader::LoadAnimation( const char* animationfilename )
{
	// �A�j���[�V�������擾
	// Importer����
	m_pFbxImp = FbxImporter::Create( m_pFbxMgr , "" );

	m_pFbxImp->Initialize( animationfilename , -1 , m_pFbxMgr->GetIOSettings() );

	// animationScene����
	m_pFbxScene = FbxScene::Create( m_pFbxMgr , "animationScene" );

	// FBX���p�[�X����fbxSceneAnimation�ɓǂݍ���
	m_pFbxImp->Import( m_pFbxScene );
	// �A�j���[�V�������擾

	int animStackCount = m_pFbxImp->GetAnimStackCount();				// �A�j���[�V���������擾

	for( int i = 0; i < 1; ++i )
		m_skinAnimeList.push_back( GetAnimationInfoList( m_pFbxImp->GetTakeInfo( i ) ) );

	return true;
}

CSkinAnimation CFbxLoader::GetAnimationInfoList( FbxTakeInfo* pFbxTakeInfo )
{
	CSkinAnimation skin;

	INT64 animationStartFrame;		// �J�n�t���[���ԍ�
	INT64 animationEndFrame;		// �I���t���[���ԍ�

	FbxTime importOffset = pFbxTakeInfo->mImportOffset;					// �I�t�Z�b�g����
	FbxTime startTime = pFbxTakeInfo->mLocalTimeSpan.GetStart();		// �J�n�������擾
	FbxTime stopTime = pFbxTakeInfo->mLocalTimeSpan.GetStop();			// �I���������擾

	// �A�j���[�V�����J�n���Ԏ��ԁ^�P�t���[���̎��ԂŊJ�n�t���[�������߂Ă���
	animationStartFrame =
		( importOffset.Get() + startTime.Get() ) / FbxTime::GetOneFrameValue( FbxTime::eFrames60 );
	// �A�j���[�V�����I�����ԁ^�P�t���[���̎��Ԃ̎��ԂŏI���t���[�������߂Ă���
	animationEndFrame =
		( importOffset.Get() + stopTime.Get() ) / FbxTime::GetOneFrameValue( FbxTime::eFrames60 );
	
	int minus = 0;
	if( animationStartFrame < 0 )
	{

	}

	skin.Initialize( ( int )animationStartFrame , ( int )animationEndFrame );

	// �m�[�h������m�[�hID���擾�ł���悤�Ɏ����ɓo�^
	int nodeCount = m_pFbxScene->GetNodeCount();


	for( int i = animationStartFrame; i < animationEndFrame; ++i )
	{
		if( i < 0 )
			continue;

		FbxTime time;
		time.Set( FbxTime::GetOneFrameValue( FbxTime::eFrames60 ) * i );	// �w��t���[�����ɉ������o�ߎ��Ԃ��Z�b�g
		AnimationFrameInfo info;
		info.m_frame = i;
		// ���̃��b�V���Ɋ܂܂�Ă���{�[���������[�v
		for( int j = 0; j < nodeCount; ++j )
		{
			FbxNode* boneNode = m_pFbxScene->GetNode( j );			// �A�j���[�V�����m�[�h�擾

			if( IsFbxNodeAttributeSkelton( boneNode ) == false )
				continue;

			info.m_name = boneNode->GetName();
			FbxMatrix boneMatrix = boneNode->EvaluateLocalTransform( time );		// �o�ߎ��Ԃɉ������p��������킷�{�[���s����擾
			info.m_matrix = FbxMtxToCMtx( boneMatrix );
			skin.Insert( info.m_frame , info.m_name.c_str() , info.m_matrix );
		}
		
	}
	//skin.m_animationInfo = infoList;
	return skin;
}