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

//  ノードがスケルトンか？
bool    IsFbxNodeAttributeSkelton( const FbxNode* pNode )
{
	if( pNode == nullptr ) 
		return false;

	const FbxNodeAttribute* pAttr = pNode->GetNodeAttribute();

	if( pAttr == nullptr ) 
		return false;

	return  pAttr->GetAttributeType() == FbxNodeAttribute::eSkeleton;
}

//! コンストラクタ
CFbxLoader::CFbxLoader(): 
	m_pFbxMgr(nullptr),
	m_pFbxImp(nullptr),
	m_pFbxScene(nullptr)
{
	m_pFbxMgr = FbxManager::Create();
	m_pFbxImp = FbxImporter::Create( m_pFbxMgr , "imp" );
	m_pFbxScene = FbxScene::Create( m_pFbxMgr , "scene" );
}

//! デストラクタ
CFbxLoader::~CFbxLoader()
{
	//! fbx sdk の仕様のよるとManagerを開放することですべて解放される
	FbxSafeDestory( m_pFbxMgr );
}

//!*******************************************************
//!	[メソッド] Fbxデータをロードする
//!
//! 引数
//!		const char*	ファイルの名前
//! 戻値
//!		bool	true 成功　: false 失敗
bool CFbxLoader::Load( const char * fn )
{
	m_pFbxImp->Initialize( fn );
	m_pFbxImp->Import( m_pFbxScene );

	// ポリゴンを三角形に
	FbxGeometryConverter geometryConverter( m_pFbxMgr );
	geometryConverter.Triangulate( m_pFbxScene , true );

	FbxNode* pNode = m_pFbxScene->GetRootNode();

	//ノードツリーの初期化
	m_nodeTree.clear();

	// シーンに含まれるメッシュ・マテリアルの解析
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

	// ノード名からノードIDを取得できるように辞書に登録
	for( int i = 0; i < nodeCount; i++ )
	{
		FbxNode* fbxNode = m_pFbxScene->GetNode( i );
		m_nodeTree.insert( pair<string , int>( fbxNode->GetName() , i ) );

		//ボーン階層構造の作成
		CTransForm* t = new CTransForm(FbxMtxToCMtx( fbxNode->EvaluateLocalTransform()) );
		t->m_name = fbxNode->GetName();

		m_boneMatrixTree.insert( pair<string , CTransForm*>( fbxNode->GetName() ,t  ));

		//メッシュ用
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

	// ワンスキンの数を取得
	int skin = mesh->GetDeformerCount( FbxDeformer::eSkin );		// ワンスキン情報の数をセット
	modelMesh->m_name = fbxnode->GetName();			
	CMaterialBase material = m_materialList[m_materialTree[fbxnode->GetMaterial( 0 )->GetName()]];
	modelMesh->m_material = material;

	//モデルに行列を設定
	modelMesh.SetTransform( m_boneMatrixTree[fbxnode->GetName()] );

	// ベースポーズの逆行列を作成しておく(メッシュの)
	FbxMatrix baseposeMatrix = fbxnode->EvaluateGlobalTransform().Inverse();

	//fbx の行列は　double　なので、　float で扱う
	double* baseposeMatrixPtr = ( double* )baseposeMatrix;
	for( int i = 0; i < 16; ++i )
	{
		modelMesh->m_inversMatrix.m[0][i] = 
			static_cast<float>( baseposeMatrixPtr[i] );
	}

	// インデックスＶＥＣＴＯＲ取得
	std::vector<int> indexList = GetIndexList( mesh );

	// 頂点データ取得
	std::vector<Vector3> positionList	= GetPositionList( mesh , indexList );		// 頂点座標を取得（面の構成情報に合わせる）
	std::vector<Vector3> normalList		= GetNormalList( mesh , indexList );		// 法線ベクトルを取得（面の構成情報に合わせる）
	std::vector<Vector3> tangentList	= GetTangentList( mesh , indexList );
	std::vector<Vector2> uv0List		= GetUVList( mesh , indexList );			// UV座標を取得（面の構成情報に合わせる）
	std::vector<CMeshVertex>  tmpmodelVertexList;

	std::vector<string> boneNameList;
	std::vector<Matrix> invBone;

	// ウエイト値を取得する
	std::vector<ModelBoneWeight> boneWeightList;
	GetWeight( mesh ,											// メッシュ（入力）
			   indexList ,										// 面の構成情報（入力）
			   boneWeightList ,									// ボーン情報リスト（出力値）（ボーン情報へのインデックス値とウェイト値）
			   boneNameList);										// ボーンオフセット行列をＶＥＣＴＯＲに保存（）

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

	

	// 頂点インデックス数分（面の構成情報順）ループする
	for( unsigned int i = 0; i < indexList.size(); i++ )
	{
		CMeshVertex vertex;
		vertex.m_pos = positionList[i];			// 頂点情報
		vertex.m_pos.w = 1;
		vertex.m_normal = normalList[i];				// 法線情報
		if( tangentList.empty() == false )
			vertex.m_tangent = tangentList[i];

		if( uv0List.empty() == false )
			vertex.m_texture = uv0List[i];
		// ボーンウェイト値が存在するか？
		if( boneWeightList.size() > 0 )
		{
			// ボーンのインデックス番号をセット
			for( int j = 0; j < 4; ++j )
			{
				vertex.m_weight.m_boneIndex[j] = boneWeightList[i].m_boneIndex[j];
			}
			// ウェイト値をセット
			vertex.m_weight.m_boneWeight = boneWeightList[i].m_boneWeight;
		}

		tmpmodelVertexList.push_back( vertex );			// 
	}

	std::vector<CMeshVertex> finalVertexList;
	std::vector<int>		 finalIndexList;
	// 重複頂点を除いてインデックス作成
	finalVertexList.reserve( tmpmodelVertexList.size() );		// 頂点情報数分エリアを確保する

	finalIndexList.reserve( indexList.size() );		// 面の構成情報数分エリアを確保する

	// ここで重複している頂点データを除き、頂点データリストとインデックスリストを作り直している
	// 重複した頂点を含んだ頂点数分ループしている(面の構成情報を元に抜き出した頂点情報)
	for( unsigned int i = 0; i < tmpmodelVertexList.size(); i++ )
	{
		CMeshVertex& vertex = tmpmodelVertexList[i];

		// 重複しているか？
		// modelMesh.vertexListは、最初空でだんだん登録されていく（重複していない頂点情報として）
		auto it = std::find( finalVertexList.begin() , finalVertexList.end() , vertex );
		if( it == finalVertexList.end() )
		{
			// 重複していない
			finalIndexList.push_back( finalVertexList.size() );	// 頂点インデックスを格納
			finalVertexList.push_back( vertex );					// 頂点情報を格納
		}
		else
		{
			// 重複している
			auto index = std::distance( finalVertexList.begin() , it );	// 先頭から現イテレータ（重複頂点した先頭データを指し示している）までのインデックス番号を取得
			finalIndexList.push_back( index );								// インデックス番号（重複頂点した先頭データを指し示している）をインデックスリストにセット
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

	FbxGeometryElementUV* element = mesh->GetElementUV( 0 );					// UVセットを取得
	FbxLayerElement::EMappingMode mappingMode = element->GetMappingMode();		// マッピングモードを取得
	FbxLayerElement::EReferenceMode referenceMode = element->GetReferenceMode();	// リファレンスモードを取得	
	const FbxLayerElementArrayTemplate<int>& indexArray = element->GetIndexArray();	// ＵＶ情報を格納したＶＥＣＴＯＲ配列のインデックス配列を取得
	const FbxLayerElementArrayTemplate<FbxVector2>& directArray = element->GetDirectArray();	// ＵＶ値配列を取得

	// 頂点インデックス数分ｖｅｃｔｏｒ容量を確保
	uvList.reserve( indexList.size() );

	// 頂点に対応して格納されている場合
	if( mappingMode == FbxGeometryElement::eByControlPoint )
	{
		// 頂点座標でマッピング
		for( unsigned int i = 0; i < indexList.size(); i++ )
		{
			int index = indexList[i];			// 面の構成情報配列から頂点インデックス番号を取得

			// リファレンスモードを判定
			int uvIndex;
			if( referenceMode == FbxGeometryElement::eDirect ){		// eDirectの場合
				uvIndex = index;		//　eDirectの場合（頂点インデックスと同じインデックス値でセットされている）
			}
			else{													// eIndexToDirectの場合
				uvIndex = indexArray.GetAt( index );				// 頂点座標インデックスに対応したＵＶ情報インデックスを取得
			}

			FbxVector2 uv = directArray.GetAt( uvIndex );		// uv値をdouble型で取得
			uvList.push_back( Vector2( static_cast<float>( uv[0] ) , static_cast<float>( uv[1] ) ) );		// float値として格納
		}
	}
	// 面の構成情報に対応して格納されている場合
	else if( mappingMode == FbxGeometryElement::eByPolygonVertex )
	{
		// ポリゴンバーテックス（面の構成情報のインデックス）でマッピング
		int indexByPolygonVertex = 0;						// 面の構成情報インデックス配列のインデックス
		int polygonCount = mesh->GetPolygonCount();			// メッシュのポリゴン数を取得
		for( int i = 0; i < polygonCount; ++i )				// ポリゴン数分ループ
		{
			int polygonSize = mesh->GetPolygonSize( i );		// ｉ番目のポリゴン頂点数を取得

			// ポリゴンの頂点数分ループ
			for( int j = 0; j < polygonSize; ++j )
			{
				// リファレンスモードの判定？
				int uvIndex;
				if( referenceMode == FbxGeometryElement::eDirect ){		// eDirectの場合
					uvIndex = indexByPolygonVertex;		//　eDirectの場合（頂点インデックスと同じインデックス値でセットされている）
				}
				else{													// eIndexToDirectの場合
					uvIndex = indexArray.GetAt( indexByPolygonVertex );	// 面の構成情報インデックスに対応したＵＶ情報インデックスを取得
				}
				FbxVector2 uv = directArray.GetAt( uvIndex );

				uvList.push_back( Vector2( static_cast<float>( uv[0] ) , static_cast<float>( uv[1] ) ) );	// ｆｌｏａｔ値として格納

				++indexByPolygonVertex;						// 頂点インデックスをインクリメント
			}
		}
	}
	else
	{
		DEBUG_PRINTF( "対応していない　UV格納" );
	}

	return uvList;
}

void CFbxLoader::GetWeight( FbxMesh* mesh ,								// ＦＢＸＭＥＳＨ
							const std::vector<int>& indexList ,						// 面の構成情報（頂点インデックス番号）
							std::vector<ModelBoneWeight>& boneWeightList ,			// ウェイト値格納ＶＥＣＴＯＲ
							std::vector<std::string>& boneNodeNameList )
{
	// メッシュ内のスキン情報数を取得する
	int skinCount = mesh->GetDeformerCount( FbxDeformer::eSkin );
	if( skinCount == 0 )
	{
		return;
	}

	// スキンが1つにしか対応しない

	if( skinCount > 1 )
		DEBUG_PRINTF( "スキンが２つ以上は対応しません" );

	int controlPointsCount = mesh->GetControlPointsCount();		// メッシュの頂点数を取得

	std::vector<std::vector<std::pair<int , float>>> tmpBoneWeightList( controlPointsCount );	// ボーンインデックス値とウェイト値をペアにしたＶＥＣＴＯＲ

	// ＦｂｘＭｅｓｈからスキン情報を取り出す
	FbxSkin* skin = static_cast<FbxSkin*>( mesh->GetDeformer( 0 , FbxDeformer::eSkin ) );
	int clusterCount = skin->GetClusterCount();	// スキンからクラスタ（ボーン）数を取得

	// クラスター（ボーンの事）の数分ループ
	for( int i = 0; i < clusterCount; ++i )
	{
		FbxCluster* cluster = skin->GetCluster( i );					// i番目のクラスタ（ボーン）を取得

		// eNormalizeしか対応しない
		if( cluster->GetLinkMode() != FbxCluster::eNormalize )
		{
			DEBUG_PRINTF( "%s,対応していないクラスタ形式\n" ,cluster->GetLink()->GetName());
			continue;
		}
		
		boneNodeNameList.push_back( cluster->GetLink()->GetName() );	// ボーン名を格納
		int indexCount = cluster->GetControlPointIndicesCount();	// ボーンに所属する頂点インデックス数を取得
		int* indices = cluster->GetControlPointIndices();			// 頂点インデックス配列を取得
		double* weights = cluster->GetControlPointWeights();		// ウェイト値配列を取得

		// ボーンに所属する頂点インデックス数分ループする
		for( int j = 0; j < indexCount; ++j )
		{
			int controlPointIndex = indices[j];		// このボーンに影響を受けるj番目の頂点インデックス番号を取得
			tmpBoneWeightList[controlPointIndex].push_back( std::pair<int , float>( i , static_cast<float>( weights[j] ) ) );
		}
	}

	// コントロールポイントに対応したウェイトを作成
	std::vector<ModelBoneWeight> boneWeightListControlPoints;

	for( unsigned int j = 0; j < tmpBoneWeightList.size(); j++ ){
		// ｊ番目の頂点のウェイト値を取得
		std::vector<std::pair<int , float>>& tmpBoneWeight = tmpBoneWeightList[j];

		// ウェイト値の大きさでソート
		std::sort( tmpBoneWeight.begin() , tmpBoneWeight.end() ,
				   []( const std::pair<int , float>& weightA , const std::pair<int , float>& weightB ){ return weightA.second > weightB.second; }
		);

		// 1頂点に4つより多くウェイトが割り振られているなら影響が少ないものは無視する
		while( tmpBoneWeight.size() > 4 )
		{
			tmpBoneWeight.pop_back();
		}

		// 4つに満たない場合はダミーを挿入
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

		// ウェイトの正規化
		weight.m_boneWeight.x /= total;
		weight.m_boneWeight.y /= total;
		weight.m_boneWeight.z /= total;
		weight.m_boneWeight.w /= total;

		boneWeightListControlPoints.push_back( weight );
	}

	// インデックスで展開
	for( unsigned int j = 0; j < indexList.size(); j++ ){
		int index = indexList[j];				// j番目の頂点インデックスを取得
		boneWeightList.push_back( boneWeightListControlPoints[index] );	// j番目の頂点のボーン番号とウエイト値をセット
	}
}

std::vector<Vector3> CFbxLoader::GetNormalList( FbxMesh* mesh , const std::vector<int>& indexList ){

	int elementCount = mesh->GetElementNormalCount();			// 何個の法線情報がセットされているか

	FbxGeometryElementNormal* element = mesh->GetElementNormal( 0 );	// ０番目の法線セットを取得
	
	FbxLayerElement::EMappingMode mappingmode = element->GetMappingMode();	// マッピングモード取得
	FbxLayerElement::EReferenceMode referrenceMode = element->GetReferenceMode();	// リファレンスモード取得
	const FbxLayerElementArrayTemplate<int>& indexArray = element->GetIndexArray();	// 法線情報を格納したｖｅｃｔｏｒ配列のインデックス値配列を取得 
	const FbxLayerElementArrayTemplate<FbxVector4>& directArray = element->GetDirectArray();	// 法線情報を格納したｖｅｃｔｏｒ配列を取得

	// 法線情報ＶＥＣＴＯＲ
	std::vector<Vector3> normalList;
	normalList.reserve( indexList.size() );		// 頂点インデックス数分確保

	// 頂点座標でマッピングされている場合
	if( mappingmode == FbxGeometryElement::eByControlPoint ){
		for( unsigned int i = 0; i < indexList.size(); i++ ){
			int index = indexList[i];		// 面の構成情報配列のi番目の頂点インデックスを取得
			int normalIndex;				// 法線情報のインデックス
			// リファレンスモードを判定
			if( referrenceMode == FbxGeometryElement::eDirect ){		// eDirectの場合
				normalIndex = index;								// 頂点インデックスの位置に対応して保存されている
			}
			else{													// eIndexToDirectの場合
				normalIndex = indexArray.GetAt( index );				// 頂点座標インデックスに対応した法線情報VECTORのインデックスを取得
			}
			FbxVector4 normal = directArray.GetAt( normalIndex );		// 法線情報を取得
			if( normal[3] != 0.0f ){
				if( normal[3] == 1.0f ){
					normalList.push_back( Vector3( static_cast<float>( normal[0] ) , static_cast<float>( normal[1] ) , static_cast<float>( normal[2] ) ) );			// 同次座標へ
				}
				else{
					normalList.push_back( Vector3( static_cast<float>( normal[0] / normal[3] ) , static_cast<float>( normal[1] / normal[3] ) , static_cast<float>( normal[2] / normal[3] ) ) );		// 同次座標へ
				}
			}
			else
				normalList.push_back( Vector3( static_cast<float>( normal[0] ) , static_cast<float>( normal[1] ) , static_cast<float>( normal[2] ) ) );		// 同次座標へ
		}
	}
	else if( mappingmode == FbxGeometryElement::eByPolygonVertex ){
		// 頂点インデックス(面の構成情報)でマッピングされている場合
		int indexByPolygonVertex = 0;
		int polygonCount = mesh->GetPolygonCount();					// ポリゴン数を取得
		for( int i = 0; i < polygonCount; ++i ){
			int polygonSize = mesh->GetPolygonSize( i );				// 頂点数を取得
			int normalIndex;
			for( int j = 0; j < polygonSize; ++j ){
				// リファレンスモードを判定
				if( referrenceMode == FbxGeometryElement::eDirect ){		// eDirectの場合
					normalIndex = indexByPolygonVertex;
				}
				else{
					normalIndex = indexArray.GetAt( indexByPolygonVertex );	// eIndexToDirectの場合
				}
				FbxVector4 normal = directArray.GetAt( normalIndex );		// 法線情報を取得
				if( normal[3] != 0.0f ){
					if( normal[3] == 1.0 ){
						normalList.push_back( Vector3( static_cast<float>( normal[0] ) , static_cast<float>( normal[1] ) , static_cast<float>( normal[2] ) ) );		// 同次座標へ
					}
					else{
						normalList.push_back( Vector3( static_cast<float>( normal[0] / normal[3] ) , static_cast<float>( normal[1] / normal[3] ) , static_cast<float>( normal[2] / normal[3] ) ) );	// 同次座標へ
					}
				}
				else
					normalList.push_back( Vector3( static_cast<float>( normal[0] ) , static_cast<float>( normal[1] ) , static_cast<float>( normal[2] ) ) );		// 同次座標へ
				++indexByPolygonVertex;
			}
		}
	}
	else{
		//todo 未対応
		for( int i = 0; i < (signed)indexList.size(); ++i )
			normalList.push_back( Vector3( 1 , 1 , 1 ).Normalize() );
	}

	return normalList;
}

std::vector<Vector3> CFbxLoader::GetTangentList( FbxMesh* mesh , const std::vector<int>& indexList ){

	int elementCount = mesh->GetElementTangentCount();			// 何個の法線情報がセットされているか

	if( elementCount == 0 )
		return std::vector<Vector3>();

	FbxGeometryElementTangent* element = mesh->GetElementTangent( 0 );	// ０番目の法線セットを取得

	FbxLayerElement::EMappingMode mappingmode = element->GetMappingMode();	// マッピングモード取得
	FbxLayerElement::EReferenceMode referrenceMode = element->GetReferenceMode();	// リファレンスモード取得
	const FbxLayerElementArrayTemplate<int>& indexArray = element->GetIndexArray();	// 法線情報を格納したｖｅｃｔｏｒ配列のインデックス値配列を取得 
	const FbxLayerElementArrayTemplate<FbxVector4>& directArray = element->GetDirectArray();	// 法線情報を格納したｖｅｃｔｏｒ配列を取得

	// 法線情報ＶＥＣＴＯＲ
	std::vector<Vector3> tanList;
	tanList.reserve( indexList.size() );		// 頂点インデックス数分確保

	// 頂点座標でマッピングされている場合
	if( mappingmode == FbxGeometryElement::eByControlPoint ){
		for( unsigned int i = 0; i < indexList.size(); i++ ){
			int index = indexList[i];		// 面の構成情報配列のi番目の頂点インデックスを取得
			int normalIndex;				// 法線情報のインデックス
			// リファレンスモードを判定
			if( referrenceMode == FbxGeometryElement::eDirect ){		// eDirectの場合
				normalIndex = index;								// 頂点インデックスの位置に対応して保存されている
			}
			else{													// eIndexToDirectの場合
				normalIndex = indexArray.GetAt( index );				// 頂点座標インデックスに対応した法線情報VECTORのインデックスを取得
			}
			FbxVector4 normal = directArray.GetAt( normalIndex );		// 法線情報を取得
			if( normal[3] != 0.0f ){
				if( normal[3] == 1.0f ){
					tanList.push_back( Vector3( static_cast<float>( normal[0] ) , static_cast<float>( normal[1] ) , static_cast<float>( normal[2] ) ) );			// 同次座標へ
				}
				else{
					tanList.push_back( Vector3( static_cast<float>( normal[0] / normal[3] ) , static_cast<float>( normal[1] / normal[3] ) , static_cast<float>( normal[2] / normal[3] ) ) );		// 同次座標へ
				}
			}
			else
				tanList.push_back( Vector3( static_cast<float>( normal[0] ) , static_cast<float>( normal[1] ) , static_cast<float>( normal[2] ) ) );		// 同次座標へ
		}
	}
	else if( mappingmode == FbxGeometryElement::eByPolygonVertex ){
		// 頂点インデックス(面の構成情報)でマッピングされている場合
		int indexByPolygonVertex = 0;
		int polygonCount = mesh->GetPolygonCount();					// ポリゴン数を取得
		for( int i = 0; i < polygonCount; ++i ){
			int polygonSize = mesh->GetPolygonSize( i );				// 頂点数を取得
			int normalIndex;
			for( int j = 0; j < polygonSize; ++j ){
				// リファレンスモードを判定
				if( referrenceMode == FbxGeometryElement::eDirect ){		// eDirectの場合
					normalIndex = indexByPolygonVertex;
				}
				else{
					normalIndex = indexArray.GetAt( indexByPolygonVertex );	// eIndexToDirectの場合
				}
				FbxVector4 normal = directArray.GetAt( normalIndex );		// 法線情報を取得
				if( normal[3] != 0.0f ){
					if( normal[3] == 1.0 ){
						tanList.push_back( Vector3( static_cast<float>( normal[0] ) , static_cast<float>( normal[1] ) , static_cast<float>( normal[2] ) ) );		// 同次座標へ
					}
					else{
						tanList.push_back( Vector3( static_cast<float>( normal[0] / normal[3] ) , static_cast<float>( normal[1] / normal[3] ) , static_cast<float>( normal[2] / normal[3] ) ) );	// 同次座標へ
					}
				}
				else
					tanList.push_back( Vector3( static_cast<float>( normal[0] ) , static_cast<float>( normal[1] ) , static_cast<float>( normal[2] ) ) );		// 同次座標へ
				++indexByPolygonVertex;
			}
		}
	}
	else{
		//todo 未対応
		for( int i = 0; i < ( signed )indexList.size(); ++i )
			tanList.push_back( Vector3( 1 , 1 , 1 ).Normalize() );
	}

	return tanList;
}

std::vector<Vector3> CFbxLoader::GetPositionList( FbxMesh* mesh , const std::vector<int>& indexList ){
	static int b = 0;
	b++;
	//　頂点座標を取得
	std::vector<Vector3> positionList;					//	頂点座標格納ＶＥＣＴＯＲ 
	positionList.reserve( indexList.size() );				//  
	for( unsigned int i = 0; i < indexList.size(); i++ ){
		int index = indexList[i];						// i番目の頂点情報（面の構成上情報順に並んでいる）を取得
		FbxVector4 Position = mesh->GetControlPointAt( index );		// index番目の頂点座標を取得
		if( Position[3] != 0.0 ){
			if( Position[3] == 1.0 ){
				positionList.push_back( Vector3( static_cast<float>( Position[0] ) , static_cast<float>( Position[1] ) , static_cast<float>( Position[2] ) ) );			// 同次座標化
			}
			else{
				positionList.push_back( Vector3( static_cast<float>( Position[0] / Position[3] ) , static_cast<float>( Position[1] / Position[3] ) , static_cast<float>( Position[2] / Position[3] ) ) );		// 同次座標化
			}
		}
		else{
			positionList.push_back( Vector3( static_cast<float>( Position[0] ) , static_cast<float>( Position[1] ) , static_cast<float>( Position[2] ) ) );				// Ｗを無視して格納
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
	int polygonCount = mesh->GetPolygonCount();			// 三角形の数を取得
	std::vector<int> indexList;							// 頂点インデックス（面の構成情報）
	indexList.reserve( polygonCount * 3 );				// 3角形の数×３

	// 面の構成情報を取得する
	for( int i = 0; i < polygonCount; i++ ){
		indexList.push_back( mesh->GetPolygonVertex( i , 0 ) );		// i番目の三角形の０番目の頂点インデックスを取得
		indexList.push_back( mesh->GetPolygonVertex( i , 1 ) );		// i番目の三角形の１番目の頂点インデックスを取得
		indexList.push_back( mesh->GetPolygonVertex( i , 2 ) );		// i番目の三角形の２番目の頂点インデックスを取得
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
	// アニメーション情報取得
	// Importer生成
	m_pFbxImp = FbxImporter::Create( m_pFbxMgr , "" );

	m_pFbxImp->Initialize( animationfilename , -1 , m_pFbxMgr->GetIOSettings() );

	// animationScene生成
	m_pFbxScene = FbxScene::Create( m_pFbxMgr , "animationScene" );

	// FBXをパースしてfbxSceneAnimationに読み込む
	m_pFbxImp->Import( m_pFbxScene );
	// アニメーション数取得

	int animStackCount = m_pFbxImp->GetAnimStackCount();				// アニメーション数を取得

	for( int i = 0; i < 1; ++i )
		m_skinAnimeList.push_back( GetAnimationInfoList( m_pFbxImp->GetTakeInfo( i ) ) );

	return true;
}

CSkinAnimation CFbxLoader::GetAnimationInfoList( FbxTakeInfo* pFbxTakeInfo )
{
	CSkinAnimation skin;

	INT64 animationStartFrame;		// 開始フレーム番号
	INT64 animationEndFrame;		// 終了フレーム番号

	FbxTime importOffset = pFbxTakeInfo->mImportOffset;					// オフセット時間
	FbxTime startTime = pFbxTakeInfo->mLocalTimeSpan.GetStart();		// 開始時刻を取得
	FbxTime stopTime = pFbxTakeInfo->mLocalTimeSpan.GetStop();			// 終了時刻を取得

	// アニメーション開始時間時間／１フレームの時間で開始フレームを求めている
	animationStartFrame =
		( importOffset.Get() + startTime.Get() ) / FbxTime::GetOneFrameValue( FbxTime::eFrames60 );
	// アニメーション終了時間／１フレームの時間の時間で終了フレームを求めている
	animationEndFrame =
		( importOffset.Get() + stopTime.Get() ) / FbxTime::GetOneFrameValue( FbxTime::eFrames60 );
	
	int minus = 0;
	if( animationStartFrame < 0 )
	{

	}

	skin.Initialize( ( int )animationStartFrame , ( int )animationEndFrame );

	// ノード名からノードIDを取得できるように辞書に登録
	int nodeCount = m_pFbxScene->GetNodeCount();


	for( int i = animationStartFrame; i < animationEndFrame; ++i )
	{
		if( i < 0 )
			continue;

		FbxTime time;
		time.Set( FbxTime::GetOneFrameValue( FbxTime::eFrames60 ) * i );	// 指定フレーム数に応じた経過時間をセット
		AnimationFrameInfo info;
		info.m_frame = i;
		// このメッシュに含まれているボーン数分ループ
		for( int j = 0; j < nodeCount; ++j )
		{
			FbxNode* boneNode = m_pFbxScene->GetNode( j );			// アニメーションノード取得

			if( IsFbxNodeAttributeSkelton( boneNode ) == false )
				continue;

			info.m_name = boneNode->GetName();
			FbxMatrix boneMatrix = boneNode->EvaluateLocalTransform( time );		// 経過時間に応じた姿勢をあらわすボーン行列を取得
			info.m_matrix = FbxMtxToCMtx( boneMatrix );
			skin.Insert( info.m_frame , info.m_name.c_str() , info.m_matrix );
		}
		
	}
	//skin.m_animationInfo = infoList;
	return skin;
}