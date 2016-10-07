#pragma once
//! BlendShape
class TsFbxShape :  public TsFbxObject,
					public TsNameObject
{
public:
	class BlendShapeLayer
	{
		friend class TsFbxShape;
	public:
		TsFbxBlendShapeKey& GetKey( TsInt index )
		{
			return m_keys[index];
		}
		TsInt		  GetKyeCount()
		{
			return m_keys.size();
		}
		TsFbxShapeArray& GetShape( TsInt index )
		{
			return m_shape[index];
		}
		TsVector<TsFbxShapeArray> GetShapeList()const
		{
			return m_shape;
		}
	protected:
		TsVector<TsFbxBlendShapeKey> m_keys;
		TsVector<TsFbxShapeArray>	m_shape;
	};

	TsFbxShape( TsFbxContext * , TsFbxScene * );
	TsBool ParseBlendShape( FbxMesh  * ,
							FbxAnimLayer * );
	TsVector<TsFbxShape::BlendShapeLayer> GetBlendShapeKeys( )const;
	TsVector<TsFbxShapeArray> GetShapeLayer( TsInt index )const;
	TsInt GetShapeLayerCount()const
	{
		return m_blendShapeLayers.size();
	}

	TsBool ConvertFinalIndex( const TsVector<TsFbxFace>& );

protected:
	TsVector<BlendShapeLayer> m_blendShapeLayers;
};