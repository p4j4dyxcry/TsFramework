#include "../../TsUT.h"
#include "TsFbxHeader.h"

TsFbxMesh::TsFbxMesh(TsFbxContext* pFbxContext) : TsFbxNode(pFbxContext)
{
	m_faceList.reserve(65535);
	m_vertexList.reserve(65535);


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
		TsInt positionCount = pFbxMesh->GetDeformerCount();
		FbxVector4* fbxPosList = pFbxMesh->GetControlPoints();
		posList.resize(positionCount);
		for (TsInt i = 0; i < positionCount; ++i)
		{
			posList[i][0] = static_cast<TsF32>(fbxPosList[i][0]);
			posList[i][1] = static_cast<TsF32>(fbxPosList[i][1]);
			posList[i][2] = static_cast<TsF32>(fbxPosList[i][2]);
			if (fbxPosList[i][3] != 0.0f && fbxPosList[i][3] != 1.0f)
				posList[i] /= (TsF32)fbxPosList[i][3];
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
		TsInt iNumUVLayers = pFbxMesh->GetElementUVCount();
		if (iNumUVLayers>TS_FBX_MAX_UV)
			iNumUVLayers = TS_FBX_MAX_UV; 

		for (TsInt i = 0; i<TS_FBX_MAX_UV; i++)
		{
			pFBXUVLayers[i] = pFbxMesh->GetElementUV(i);
		}

		for (TsInt i = 0; i<iNumUVLayers; i++)
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
		for (TsInt i = 0; i < TS_FBX_MAX_UV; ++i)
			MappingByFace(pFBXUVLayers[i], 12 + i * 3);
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
	return stride;
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