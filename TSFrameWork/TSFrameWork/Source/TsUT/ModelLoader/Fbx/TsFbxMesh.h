#pragma once

class TsFbxMesh : public TsFbxNode
{
public:
	TsFbxMesh(TsFbxContext* pFbxContext, TsFbxScene* pFbxScene);

	enum VertexFormat
	{
		POS				= 0x0001,
		NORMAL			= 0x0002,			
		BONE_WEIGHT		= 0x0004,
		BONE_INDEX		= 0x0008,
		TANGENT			= 0x0010,
		BINORMAL		= 0x0020,
		VERTEX_COLOR	= 0x0040,
		UV0				= 0x0080,
		UV1				= 0x0100,
		UV2				= 0x0200,
		UV3				= 0x0400,
		UV4				= 0x0800,
		UV5				= 0x1000,
	};
	static const VertexFormat DefaultVertexFormat = (VertexFormat)(VertexFormat::POS | VertexFormat::NORMAL | VertexFormat::UV0 );
	static const VertexFormat DefaultSkinVertexFormat 
		= (VertexFormat)( DefaultVertexFormat     |
						 VertexFormat::BONE_WEIGHT|
						 VertexFormat::BONE_INDEX);

	TsBool SetVertexFormat(VertexFormat format);
	TsBool AddVertexFormat(VertexFormat format);
	TsBool RemoveVertexFormat(VertexFormat format);

	void*  CreateVertexBuffer()const;
	size_t GetVertexStride()const;
	TsInt  GetVertexCount()const;

	void*	CreateIndexBuffer()const;
	size_t	GetIndexBufferSize()const;
	virtual ~TsFbxMesh(){};

	TsBool ParseFbxMesh();

protected:
	template<typename T>
	TsBool MappingToFace(T* p, TsInt startIndex);

	TsBool ParseSkin( FbxSkin* pFbxSkin , TsInt vertexCount,
					  TsVector<TsInt4>&		boneIndexList,
					  TsVector<TsFloat4>&	boneWeightList);

	TsVector<TsFbxFace>		m_faceList;
	TsVector<TsFbxVertex>	m_vertexList;
	TsVector<TsInt>			m_indexList;
	VertexFormat m_vertexBufferFormat;
	TsVector<TsFbxShape>	m_BlendShapeChannelList;
	TsInt m_uvLayerCount;
};

