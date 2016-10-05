#include "../../TsUT.h"
#include "TsFbxHeader.h"

TsFbxMesh::TsFbxMesh(TsFbxContext* pFbxContext, TsFbxScene* pFbxScene) : TsFbxNode(pFbxContext,pFbxScene)
{
	m_faceList.reserve(65535);
	m_vertexList.reserve(65535);

	m_uvLayerCount = 0;
	m_vertexBufferFormat = DefaultVertexFormat;
}

TsBool TsFbxMesh::SetVertexFormat(VertexFormat format)
{
	m_vertexBufferFormat = format;
	return TS_TRUE;
}
TsBool TsFbxMesh::AddVertexFormat(VertexFormat format)
{
	m_vertexBufferFormat = (VertexFormat)(m_vertexBufferFormat | format);
	return TS_TRUE;
}
TsBool TsFbxMesh::RemoveVertexFormat(VertexFormat format)
{
	m_vertexBufferFormat = (VertexFormat)(m_vertexBufferFormat - format);
	return TS_TRUE;
}

TsBool TsFbxMesh::Perse()
{
	FbxMesh * pFbxMesh = AsAttributeFbxMesh();
	if (pFbxMesh == nullptr)
		return TS_FALSE;

	//==============================================================
	//! 位置情報の読み込み
	//==============================================================
	TsVector<TsFloat3> posList;
	{
		TsInt positionCount = pFbxMesh->GetControlPointsCount();
		FbxVector4* fbxPosList = pFbxMesh->GetControlPoints();
		posList.resize(positionCount);
		for (TsInt i = 0; i < positionCount; ++i)
		{
			posList[i][0] = static_cast<TsF32>(fbxPosList[i][0]);
			posList[i][1] = static_cast<TsF32>(fbxPosList[i][1]);
			posList[i][2] = static_cast<TsF32>(fbxPosList[i][2]);
			
			TsVector3 v = TsVector3(posList[i].x, posList[i].y, posList[i].z);
			posList[i] = m_geometricTransform.ToWorldMatrix().TransformPoint(v);
		}
	}

	//==============================================================
	//! 法線の読み込み
	//==============================================================
	TsVector<TsFloat3> normalList;
	FbxGeometryElementNormal*      pFBXNormals = pFbxMesh->GetElementNormal();
	{
		if (pFBXNormals != nullptr)
		{
			FbxLayerElementArrayTemplate<FbxVector4> &vArray = pFBXNormals->GetDirectArray();
			normalList.resize(vArray.GetCount());

			FbxVector4 *pNormals = NULL;
			pNormals = vArray.GetLocked(pNormals, vArray.eReadLock);
			if (pNormals != nullptr)
			{
				for (TsUint i = 0; i<normalList.size(); i++)
				{
					const FbxVector4 &normal = pNormals[i];
					normalList[i][0] = static_cast<TsF32>(normal[0]);
					normalList[i][1] = static_cast<TsF32>(normal[1]);
					normalList[i][2] = static_cast<TsF32>(normal[2]);
				} 
				vArray.Release(&pNormals, pNormals);
			}
		}
	}

	//==============================================================
	//! タンジェントの読み込み
	//==============================================================
	TsVector<TsFloat3> tangentList;
	FbxGeometryElementTangent*     pFBXTangents = pFbxMesh->GetElementTangent();
	{
		if (pFBXTangents != NULL)
		{
			FbxLayerElementArrayTemplate<FbxVector4> &vArray = pFBXTangents->GetDirectArray();
			tangentList.resize(vArray.GetCount());

			FbxVector4 *pTangents = NULL;
			pTangents = vArray.GetLocked(pTangents, vArray.eReadLock);
			if (pTangents)
			{
				for (TsUint i = 0; i<tangentList.size(); i++)
				{
					FbxVector4 tangent = pTangents[i];
					tangentList[i][0] = static_cast<TsF32>(tangent[0]);
					tangentList[i][1] = static_cast<TsF32>(tangent[1]);
					tangentList[i][2] = static_cast<TsF32>(tangent[2]);
				}
				vArray.Release(&pTangents, pTangents);
			}
		}
	}

	//==============================================================
	//! バイノーマルの読み込み
	//==============================================================
	TsVector<TsFloat3> binormalList;
	FbxGeometryElementBinormal*    pFBXBinormals = pFbxMesh->GetElementBinormal();
	{
		if (pFBXBinormals != NULL)
		{
			FbxLayerElementArrayTemplate<FbxVector4> &vArray = pFBXBinormals->GetDirectArray();
			binormalList.resize(vArray.GetCount());

			FbxVector4 *pBinormals = NULL;
			pBinormals = vArray.GetLocked(pBinormals, vArray.eReadLock);
			if (pBinormals)
			{
				for (TsUint i = 0; i<binormalList.size(); i++)
				{
					FbxVector4 binormal = pBinormals[i];
					binormalList[i][0] = static_cast<TsF32>(binormal[0]);
					binormalList[i][1] = static_cast<TsF32>(binormal[1]);
					binormalList[i][2] = static_cast<TsF32>(binormal[2]);
				} 
			}
			vArray.Release(&pBinormals, pBinormals);
		} 
	}

	//==============================================================
	//! Vertex Color の読み込み
	//==============================================================
	TsVector<TsColor> vertexColorList;
	FbxGeometryElementVertexColor* pFBXVertexColors = pFbxMesh->GetElementVertexColor();
	{
		if (pFBXVertexColors != NULL)
		{
			FbxLayerElementArrayTemplate<FbxColor> &vArray = pFBXVertexColors->GetDirectArray();
			vertexColorList.resize(vArray.GetCount());

			FbxColor *pVertexColors = NULL;
			pVertexColors = vArray.GetLocked(pVertexColors, vArray.eReadLock);
			if (pVertexColors)
			{
				for (TsUint i = 0; i<vertexColorList.size(); i++)
				{
					FbxColor vertexColor = pVertexColors[i];
					vertexColorList[i].m_color[0] = (TsF32)vertexColor.mRed;
					vertexColorList[i].m_color[1] = (TsF32)vertexColor.mGreen;
					vertexColorList[i].m_color[2] = (TsF32)vertexColor.mBlue;
					vertexColorList[i].m_color[3] = (TsF32)vertexColor.mAlpha;
				} 
				vArray.Release(&pVertexColors, pVertexColors);
			}
		} 
	}

	//==============================================================
	//! UVレイヤの読み込み
	//==============================================================
	TsVector<TsFloat2> uvList[TS_FBX_MAX_UV];
	FbxGeometryElementUV* pFBXUVLayers[TS_FBX_MAX_UV];

	{
		TsInt m_uvLayerCount = pFbxMesh->GetElementUVCount();
		if (m_uvLayerCount > TS_FBX_MAX_UV)
		{
			m_uvLayerCount = TS_FBX_MAX_UV;
		}

		for (TsInt i = 0; i<TS_FBX_MAX_UV; i++)
		{
			pFBXUVLayers[i] = pFbxMesh->GetElementUV(i);
		}

		for (TsInt i = 0; i<m_uvLayerCount; i++)
		{
			FbxGeometryElementUV* pFBXUVs = pFBXUVLayers[i];

			FbxLayerElementArrayTemplate<FbxVector2> &vArray = pFBXUVs->GetDirectArray();
			uvList[i].resize(vArray.GetCount());

			FbxVector2 *pUVCoords = NULL;
			pUVCoords = vArray.GetLocked(pUVCoords, vArray.eReadLock);

			for (TsUint j = 0; j<uvList[i].size(); j++)
			{
				FbxVector2 uvCoord = pUVCoords[j];
				uvList[i][j].x = (TsF32)uvCoord[0];
				uvList[i][j].y = (TsF32)uvCoord[1];
			} 
			vArray.Release(&pUVCoords, pUVCoords);
		}
	}

	//==============================================================
	// 三角面の生成
	//==============================================================
	{
		m_faceList.resize(pFbxMesh->GetPolygonCount());
		memset(&m_faceList[0], 0, (sizeof(TsFbxFace)*m_faceList.size()));

		for (TsUint i = 0; i<m_faceList.size(); i++)
		{
			TsInt iPolySize = pFbxMesh->GetPolygonSize(i);

			TsFbxFace &face = m_faceList[i];
			for (TsInt j = 0; j<iPolySize; j++)
			{
				face.posIndex[j] = pFbxMesh->GetPolygonVertex(i, j );
			}
		} 
	}

	//--------------------------------------------------------------------------
	// 面に法線インデックスをバインドする
	//--------------------------------------------------------------------------
	if (pFBXNormals != NULL)
	{
		MappingByFace( pFBXNormals, 3);
	} 

	//--------------------------------------------------------------------------
	// 面にTangentインデックスをバインドする
	//--------------------------------------------------------------------------
	if (pFBXTangents != NULL)
	{
		MappingByFace(pFBXTangents, 6);
	}

	//--------------------------------------------------------------------------
	// 面にBinormalインデックスをバインドする
	//--------------------------------------------------------------------------
	if (pFBXBinormals != NULL)
	{
		MappingByFace(pFBXBinormals, 9);
	}

	//--------------------------------------------------------------------------
	// 面にVertexColorインデックスをバインドする
	//--------------------------------------------------------------------------
	if (pFBXVertexColors != NULL)
	{
		MappingByFace(pFBXVertexColors, 12);
	}

	//--------------------------------------------------------------------------
	// 面にLayeredUVインデックスをバインドする
	//--------------------------------------------------------------------------
	if (pFBXVertexColors != NULL)
	{
		for (TsInt i = 0; i < m_uvLayerCount; ++i)
			MappingByFace(pFBXUVLayers[i], 12 + i * 3);
	}

	//--------------------------------------------------------------------------
	// スキン情報を読み込む
	//--------------------------------------------------------------------------
	TsVector<TsInt4>   boneIndexList;
	TsVector<TsFloat4> boneWeightList;
	TsInt skinCount = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (skinCount>0)
	{
		FbxSkin* pFbxSkin = (FbxSkin*)pFbxMesh->GetDeformer(0, FbxDeformer::eSkin);
		PerseSkin(pFbxSkin, (TsInt)posList.size(), boneIndexList, boneWeightList);
	} 

	TsInt shapeCount = pFbxMesh->GetDeformerCount(FbxDeformer::eBlendShape);
	if (shapeCount > 0)
	{
		FbxBlendShape* pBlendShape = (FbxBlendShape*)pFbxMesh->GetDeformer(0, FbxDeformer::eBlendShape);
		TsInt channelCount = pBlendShape->GetBlendShapeChannelCount();
		printf("%s\n", GetName().c_str());
		for (int i = 0; i < channelCount; ++i)
		{
			FbxBlendShapeChannel* shapeCH = pBlendShape->GetBlendShapeChannel(i);
			TsInt shapeCount = shapeCH->GetTargetShapeCount();
			for (TsInt j = 0; j < shapeCount; ++j)
			{
				FbxShape * pShape =shapeCH->GetTargetShape(j);
				pShape->GetControlPoints();
			}
		}
	}

	//--------------------------------------------------------------------------
	// 頂点フォーマットの作成
	//--------------------------------------------------------------------------
	m_vertexList.reserve(m_faceList.size() * 3);
	for (TsUint i = 0; i < m_faceList.size(); ++i)
	{
		for (TsUint j = 0; j < 3; ++j)
		{
			TsFbxVertex vertex;
			if (!posList.empty())
				vertex.pos = posList[m_faceList[i].posIndex[j]];
			if (!normalList.empty())
				vertex.normal = normalList[m_faceList[i].normalIndex[j]];
			if (!tangentList.empty())
				vertex.tangent = tangentList[m_faceList[i].tangentIndex[j]];
			if (!binormalList.empty())
				vertex.binormal = binormalList[m_faceList[i].binormalIndex[j]];
			if (!vertexColorList.empty())
				vertex.color = vertexColorList[m_faceList[i].colorIndex[j]];
			for (TsInt k = 0; k < m_uvLayerCount; ++k)
				vertex.uv[k] = uvList[m_faceList[i].UVIndex[j][k]][k];
			if (!boneWeightList.empty())
				vertex.boneWeight = boneWeightList[m_faceList[i].posIndex[j]];
			if (!boneIndexList.empty())
				vertex.boneIndex = boneIndexList[m_faceList[i].posIndex[j]];
			

			// 重複しているか？
			// modelMesh.vertexListは、最初空でだんだん登録されていく（重複していない頂点情報として）
			auto it = std::find(m_vertexList.begin(), m_vertexList.end(), vertex);
			if (it == m_vertexList.end())
			{
				// 重複していない
				m_faceList[i].finalIndex[j] = m_vertexList.size();	// 頂点インデックスを格納
				m_vertexList.push_back(vertex);					// 頂点情報を格納
			}
			else
			{
				// 重複している
				auto index = std::distance(m_vertexList.begin(), it);	// 先頭から現イテレータ（重複頂点した先頭データを指し示している）までのインデックス番号を取得
				m_faceList[i].finalIndex[j] = index;							// インデックス番号（重複頂点した先頭データを指し示している）をインデックスリストにセット
			}
		}
	}

	return TS_TRUE;
}

void*  TsFbxMesh::CreateVertexBuffer()const
{
	TsInt stride = GetVertexStride();
	void * pData = TsNew(void*[stride * m_vertexList.size()]);
	TsF32 * ptr = (TsF32*)pData;
	for (TsUint i = 0; i < m_vertexList.size(); ++i)
	{
		if (m_vertexBufferFormat & VertexFormat::POS)
		{
			memcpy(ptr, (float*)&m_vertexList[i].pos, sizeof(TsF32)* 3);
			ptr += 3;
		}
		if (m_vertexBufferFormat & VertexFormat::NORMAL)
		{
			memcpy(ptr, (float*)&m_vertexList[i].normal, sizeof(TsF32)* 3);
			ptr += 3;
		}if (m_vertexBufferFormat & VertexFormat::BONE_WEIGHT)
		{
			memcpy(ptr, (float*)&m_vertexList[i].boneWeight, sizeof(TsF32)* 4);
			ptr += 4;
		}
		if (m_vertexBufferFormat & VertexFormat::BONE_INDEX)
		{
			memcpy(ptr, (float*)&m_vertexList[i].boneIndex, sizeof(TsInt)* 4);
			ptr += 4;
		}
		if (m_vertexBufferFormat & VertexFormat::TANGENT)
		{
			memcpy(ptr, (float*)&m_vertexList[i].tangent, sizeof(TsF32)* 3);
			ptr += 3;
		}
		if (m_vertexBufferFormat & VertexFormat::BINORMAL)
		{
			memcpy(ptr, (float*)&m_vertexList[i].binormal, sizeof(TsF32)* 3);
			ptr += 3;
		}
		if (m_vertexBufferFormat & VertexFormat::VERTEX_COLOR)
		{
			memcpy(ptr, (float*)&m_vertexList[i].color, sizeof(TsF32)* 4);
			ptr += 4;
		}
		if (m_vertexBufferFormat & VertexFormat::UV0)
		{
			memcpy(ptr, (float*)&m_vertexList[i].uv[0], sizeof(TsF32)* 2);
			ptr += 2;
		}
		if (m_vertexBufferFormat & VertexFormat::UV1)
		{
			memcpy(ptr, (float*)&m_vertexList[i].uv[1], sizeof(TsF32)* 2);
			ptr += 2;
		}
		if (m_vertexBufferFormat & VertexFormat::UV2)
		{
			memcpy(ptr, (float*)&m_vertexList[i].uv[2], sizeof(TsF32)* 2);
			ptr += 2;
		}
		if (m_vertexBufferFormat & VertexFormat::UV3)
		{
			memcpy(ptr, (float*)&m_vertexList[i].uv[3], sizeof(TsF32)* 2);
			ptr += 2;
		}
		if (m_vertexBufferFormat & VertexFormat::UV4)
		{
			memcpy(ptr, (float*)&m_vertexList[i].uv[4], sizeof(TsF32)* 2);
			ptr += 2;
		}
		if (m_vertexBufferFormat & VertexFormat::UV5)
		{
			memcpy(ptr, (float*)&m_vertexList[i].uv[5], sizeof(TsF32)* 2);
			ptr += 2;
		}
	}

	return pData;
}
size_t TsFbxMesh::GetVertexStride()const
{
	TsInt stride = 0;
	if (m_vertexBufferFormat & VertexFormat::POS)
		stride += 3;
	if (m_vertexBufferFormat & VertexFormat::NORMAL)
		stride += 3;
	if (m_vertexBufferFormat & VertexFormat::BONE_WEIGHT)
		stride += 4;
	if (m_vertexBufferFormat & VertexFormat::BONE_INDEX)
		stride += 4;
	if (m_vertexBufferFormat & VertexFormat::TANGENT)
		stride += 3;
	if (m_vertexBufferFormat & VertexFormat::BINORMAL)
		stride += 3;
	if (m_vertexBufferFormat & VertexFormat::VERTEX_COLOR)
		stride += 4;
	if (m_vertexBufferFormat & VertexFormat::UV0)
		stride += 2;
	if (m_vertexBufferFormat & VertexFormat::UV1)
		stride += 2;
	if (m_vertexBufferFormat & VertexFormat::UV2)
		stride += 2;
	if (m_vertexBufferFormat & VertexFormat::UV3)
		stride += 2;
	if (m_vertexBufferFormat & VertexFormat::UV4)
		stride += 2;
	if (m_vertexBufferFormat & VertexFormat::UV5)
		stride += 2;
	return stride * sizeof(TsF32);
}


template<typename T> 
TsBool TsFbxMesh::MappingByFace( T* geometory,
								 TsInt startIndex)
{
	FbxGeometryElement::EMappingMode mappingMode = geometory->GetMappingMode();
	FbxGeometryElement::EReferenceMode refMode = geometory->GetReferenceMode();
	TsInt *pIndex = NULL;
	if (refMode == FbxGeometryElement::eIndexToDirect)
	{
		FbxLayerElementArrayTemplate<TsInt> &vArray = geometory->GetIndexArray();
		pIndex = vArray.GetLocked(pIndex, vArray.eReadLock);
	}
	if (mappingMode == FbxGeometryElement::eByControlPoint)
	{
		TsVector<TsInt> indicesPerControlPoints;
		indicesPerControlPoints.resize(AsAttributeFbxMesh()->GetControlPointsCount());

		for (TsInt i = 0; i<AsAttributeFbxMesh()->GetControlPointsCount(); i++)
		{
			TsInt index = 0;
			if (refMode == FbxGeometryElement::eDirect)
				index = i;
			else if (refMode == FbxGeometryElement::eIndexToDirect)
				index = pIndex[i];

			indicesPerControlPoints[i] = index;
		}

		for (TsUint i = 0; i<m_faceList.size(); i++)
		{
			TsInt iPolySize = AsAttributeFbxMesh()->GetPolygonSize(i);

			TsFbxFace &face = m_faceList[i];
			for (TsInt j = 0; j<iPolySize; j++)
			{
				face.data[startIndex + j] = indicesPerControlPoints[face.posIndex[j]];
			}
		}
	}
	else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
	{
		TsInt iPolyDirectIndex = 0;

		for (TsUint i = 0; i<m_faceList.size(); i++)
		{
			TsInt iPolySize = AsAttributeFbxMesh()->GetPolygonSize(i);

			TsFbxFace &face = m_faceList[i];
			for (TsInt j = 0; j<iPolySize; j++)
			{
				TsInt index = 0;

				if (refMode == FbxGeometryElement::eDirect)
					index = iPolyDirectIndex;
				else if (refMode == FbxGeometryElement::eIndexToDirect)
					index = pIndex[iPolyDirectIndex];

				face.data[startIndex + j] = index;

				iPolyDirectIndex++;
			}
		}
	}
	if (refMode == FbxGeometryElement::eIndexToDirect)
	{
		FbxLayerElementArrayTemplate<TsInt> &vArray = geometory->GetIndexArray();
		vArray.Release(&pIndex, pIndex);
	}
	return TS_TRUE;
};

TsBool TsFbxMesh::PerseSkin(FbxSkin* pFbxSkin, TsInt vertexCount,
							TsVector<TsInt4>&		boneIndexList,
							TsVector<TsFloat4>&		boneWeightList)
{
	if (vertexCount <= 0)
		return TS_FALSE;

	boneIndexList.resize(vertexCount);
	boneWeightList.resize(vertexCount);

	TsInt i;
	for (i = 0; i<vertexCount; i++)
	{
		boneIndexList[i].x = -1;
		boneIndexList[i].y = -1;
		boneIndexList[i].z = -1;
		boneIndexList[i].w = -1;

		boneWeightList[i].x = -1.0f;
		boneWeightList[i].y = -1.0f;
		boneWeightList[i].z = -1.0f;
		boneWeightList[i].w = -1.0f;
	} // End for

	// Parse bones
	TsInt numClusters = (TsInt)pFbxSkin->GetClusterCount();

	TsVector<FbxNode*> nodes;
	nodes.resize((size_t)numClusters);

	for (i = 0; i<numClusters; i++)
	{
		FbxCluster* pCluster = pFbxSkin->GetCluster(i);

		FbxNode *pNode = pCluster->GetLink();

		nodes[i] = pNode;
	} // End for

	for (i = 0; i<numClusters; i++)
	{
		FbxNode *pFbxNode = nodes[i];

		{
			TsInt clusterIndex = i;

			// Compute bone index from FbxNode
			TsInt boneIndex = 0;

			TsFbxBone* pBone = (TsFbxBone*)m_pFbxScene->FindNodeByName(pFbxNode->GetName());

			if (pBone != NULL)
			{
				boneIndex = pBone->GetBoneIndex();
			} // End if

			if (boneIndex <0)
			{
				// Bone is not found???
				boneIndex = 0;
			} 

			FbxCluster *pCluster = pFbxSkin->GetCluster(clusterIndex);

			TsInt numPointsInfluencing = pCluster->GetControlPointIndicesCount();

			TsInt *pIndices = pCluster->GetControlPointIndices();
			TsF64 *pWeights = pCluster->GetControlPointWeights();

			TsInt   idx;
			TsF32 weight;

			TsInt j;
			for (j = 0; j<numPointsInfluencing; j++)
			{
				idx = pIndices[j];
				weight = (TsF32)pWeights[j];

				if (weight <= 0.0f)
					continue;

				// データが不正な場合があるので、その場合は、処理を飛ばす。
				if (idx >= vertexCount)
				{
					continue;
				}

				// Insert index and weight
				if (boneIndexList[idx].x<0)
				{
					boneIndexList[idx].x = (TsByte)boneIndex;
					boneWeightList[idx].x = weight;
				} 
				else if (boneIndexList[idx].y<0)
				{
					boneIndexList[idx].y = (TsByte)boneIndex;
					boneWeightList[idx].y = weight;
				} 
				else if (boneIndexList[idx].z<0)
				{
					boneIndexList[idx].z = (TsByte)boneIndex;
					boneWeightList[idx].z = weight;
				} 
				else if (boneIndexList[idx].w<0)
				{
					boneIndexList[idx].w = (TsByte)boneIndex;
					boneWeightList[idx].w = weight;
				} 
				else
				{
					// We find the smallest weight and replace it
					TsF32 minWeight = boneWeightList[idx][0];
					TsInt channelIdxToReplace = 0;
					TsInt k;
					for (k = 1; k<4; k++)
					{
						if (boneWeightList[idx][k]<minWeight)
						{
							minWeight = boneWeightList[idx][k];
							channelIdxToReplace = k;
						} 
					} 

					if (minWeight<weight)
					{
						boneIndexList[idx][channelIdxToReplace] = (TsByte)boneIndex;
						boneWeightList[idx][channelIdxToReplace] = weight;
					} 
				} 
			} 
		}
	} 

	// Now make sure we don' t have any weights in negative values
	for (i = 0; i<vertexCount; i++)
	{
		boneWeightList[i].x = max(0.0f, boneWeightList[i].x);
		boneWeightList[i].y = max(0.0f, boneWeightList[i].y);
		boneWeightList[i].z = max(0.0f, boneWeightList[i].z);
		boneWeightList[i].w = max(0.0f, boneWeightList[i].w);

		TsF32 totalWeight = boneWeightList[i].x +
			boneWeightList[i].y +
			boneWeightList[i].z +
			boneWeightList[i].w;
		if (totalWeight>0.0f)
		{
			boneWeightList[i].x /= totalWeight;
			boneWeightList[i].y /= totalWeight;
			boneWeightList[i].z /= totalWeight;
			boneWeightList[i].w /= totalWeight;
		} // End if
	} // End for

	return TS_TRUE;
}

TsInt  TsFbxMesh::GetVertexCount()const
{
	return (TsInt)m_vertexList.size();
}

void*  TsFbxMesh::CreateIndexBuffer()const
{
	TsUint * pBuffer = TsNew(TsUint[m_faceList.size() * 3]);

	for (TsInt i = 0; i < m_faceList.size(); ++i)
	for (TsInt j = 0; j < 3; ++j)
		pBuffer[i * 3 + j] = m_faceList[i].finalIndex[j];

	return pBuffer;
}

size_t  TsFbxMesh::GetIndexBufferSize()const
{
	return m_faceList.size() * 3 * sizeof(TsInt);
}