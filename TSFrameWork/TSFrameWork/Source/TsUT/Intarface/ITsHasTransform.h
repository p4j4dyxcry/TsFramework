#pragma once

class TsTransForm;

class IHasTransform
{
public:
	IHasTransform();
	virtual ~IHasTransform();

	TsVector3			GetLocalPosition()const;
	TsQuaternion		GetLocalRotate()const;
	TsVector3			GetLocalScale()const;

	TsVector3			GetWorldPosition()const;
	TsMatrix			GetWorldMatrix()const;
	const TsTransForm*	GetTransformPtr()const;
	
	TsBool				SetParent( IHasTransform* );
	TsBool				AddChild( IHasTransform* );

	TsBool				SetWorldPos(const TsVector3&);
	TsBool				SetLocalPos(const TsVector3&);

	TsBool				SetLocalRotate(const TsQuaternion&);

	TsBool				SetWorldScale(const TsVector3&);
	TsBool				SetLocalScale(const TsVector3&);

	TsBool				SetLcaoRotateAxis(const TsVector3&, TsF32 angle);
protected:
	TsTransForm* m_pTransform;
};