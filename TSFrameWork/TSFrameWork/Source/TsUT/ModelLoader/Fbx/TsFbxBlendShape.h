#pragma once
//! BlendShape

class TsFbxShape : public TsNameObject
{
public:
	struct Shape
	{
		TsInt		index;
		TsFloat3	pos;
	};
	
	TsFbxShape(FbxShape* pShape);

	TsBool ParseBlendShape( FbxMesh  * ,
							FbxAnimLayer * );

	TsVector<Shape> GetShapes()const;
protected:
	TsVector<Shape>	m_shape;
};