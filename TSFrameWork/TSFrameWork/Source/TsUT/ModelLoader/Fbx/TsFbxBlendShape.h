#pragma once
//! BlendShape

struct FBXShapeVertex
{
	TsInt		index;
	TsFloat3	pos;
};

struct FBXBlendShapeKey
{
	TsF32		time;
	TsF32		weight;
	TsInt		beginIndex;
	TsInt		endIndex;
};

#define FBX_SHAPE TsVector<FBXShapeVertex>

class TsFbxShape :  public TsFbxObject,
					public TsNameObject
{
public:
	class BlendShapeLayer
	{
		friend class TsFbxShape;
	public:
		FBXBlendShapeKey& GetKey( TsInt index )
		{
			return m_keys[index];
		}
		TsInt		  GetKyeCount()
		{
			return m_keys.size();
		}
		FBX_SHAPE& GetShape( TsInt index )
		{
			return m_shape[index];
		}
		TsVector<FBX_SHAPE> GetShapeList()const
		{
			return m_shape;
		}
	protected:
		TsVector<FBXBlendShapeKey> m_keys;
		TsVector<FBX_SHAPE>	m_shape;
	};

	TsFbxShape( TsFbxContext * , TsFbxScene * );
	TsBool ParseBlendShape( FbxMesh  * ,
							FbxAnimLayer * );
	TsVector<TsFbxShape::BlendShapeLayer> GetBlendShapeKeys( )const;
	TsVector<FBX_SHAPE> GetShapes( TsInt index )const;

protected:
	TsVector<BlendShapeLayer> m_blendShapeLayers;
};