#pragma once
//! BlendShape

class TsFbxShape :  public TsFbxObject,
					public TsNameObject
{
public:
	struct BlendShapeKey
	{
		TsF32		time;
		TsF32		weight;
		TsInt		beginIndex;
		TsInt		endIndex;
	};

	class BlendShapeLayer
	{
	public:
		BlendShapeKey GetKey( TsInt index )const;
		TsInt		  GetKyeCount()const;
	protected:
		TsVector<BlendShapeKey> m_keys;
	};

	struct Shape
	{
		TsInt		index;
		TsFloat3	pos;
	};

	TsFbxShape( TsFbxContext * , TsFbxScene * );
	TsBool ParseBlendShape( FbxMesh  * ,
							FbxAnimLayer * );
	TsVector<BlendShapeKey> GetBlendShapeKeys( TsInt index )const;
	TsVector<Shape> GetShapes()const;
	

protected:
	TsVector<Shape>	m_shape;
	TsVector<TsVector<BlendShapeKey>> m_blendShapeLayers;
};