#pragma once

static const TsInt TS_FBX_MAX_UV = (6);
// fbx vertex
struct TsFbxVertex
{
	TsFloat3 pos;
	TsFloat3 normal;	//
	TsFloat4 boneWeight;
	TsInt4   boneIndex;
	TsFloat3 tangent;
	TsFloat3 binormal;
	TsColor  color;
	TsFloat2 uv[TS_FBX_MAX_UV];		//uv x 8 layer
};

struct TsFbxDefaultVertex
{
	TsFloat3 pos;
	TsFloat3 normal;
	TsFloat2 uv0;
};

struct TsFbxDefaultSkinVertex
{
	TsFloat3 pos;
	TsFloat3 normal;
	TsFloat2 uv;
	TsFloat4 weight;
	TsInt4   boneIdx;
};

// fbx face
struct TsFbxFace
{
	union
	{
		struct
		{
			TsInt posIndex[3];
			TsInt normalIndex[3];
			TsInt tangentIndex[3];
			TsInt binormalIndex[3];
			TsInt colorIndex[3];
			TsInt UVIndex[3][TS_FBX_MAX_UV];

			TsInt finalIndex[3];
		};
		TsInt data[32];
	};
};


class TsFbxMesh : public TsFbxNode
{
public:
	TsFbxMesh(TsFbxContext* pFbxContext);

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
	virtual ~TsFbxMesh(){};

	TsBool Perse();

protected:
	template<typename T>
	TsBool MappingByFace(T* p, TsInt startIndex);

	TsVector<TsFbxFace>		m_faceList;
	TsVector<TsFbxVertex>	m_vertexList;

	VertexFormat m_vertexBufferFormat;
};

