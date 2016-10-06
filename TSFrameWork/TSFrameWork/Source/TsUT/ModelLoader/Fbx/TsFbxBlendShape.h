#pragma once
//! BlendShape

class TsFbxShape :  public TsFbxObject,
					public TsNameObject
{
public:
	struct Shape
	{
		TsInt		index;
		TsFloat3	pos;
	};
	
	TsFbxShape( TsFbxContext * , TsFbxScene * );

	TsBool ParseBlendShape( FbxMesh  * ,
							FbxAnimLayer * );

	TsVector<Shape> GetShapes()const;
protected:
	TsVector<Shape>	m_shape;
};