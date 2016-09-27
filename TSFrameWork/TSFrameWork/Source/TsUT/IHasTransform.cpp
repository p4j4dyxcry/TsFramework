#include "TsUT.h"
#include "../TsMath/TsTransForm.h"

IHasTransform::IHasTransform()
:m_pTransform(nullptr)
{
	m_pTransform = TsNew( TsTransForm );
}
IHasTransform::~IHasTransform()
{
	TsSafeDelete(m_pTransform);
}

TsVector3	IHasTransform::GetLocalPosition()const
{
	return m_pTransform->m_localPosition;
}
TsQuaternion	IHasTransform::GetLocalRotate()const
{
	return m_pTransform->m_localRotate;
}
TsVector3		IHasTransform::GetLocalScale()const
{
	return m_pTransform->m_localScale;
}

TsVector3		IHasTransform::GetWorldPosition()const
{
	TsMatrix m = GetWorldMatrix();
	return  TsVector3(m._41, m._42, m._43);
}
TsMatrix			IHasTransform::GetWorldMatrix()const
{
	return m_pTransform->ToWorldMatrix();
}

const TsTransForm*	IHasTransform::GetTransformPtr()const
{
	return m_pTransform;
}

TsBool				IHasTransform::SetParent(IHasTransform* parent)
{
	m_pTransform->SetParent(parent->m_pTransform);
	return TS_TRUE;
}
TsBool				IHasTransform::AddChild(IHasTransform* child)
{
	m_pTransform->Add(child->m_pTransform);
	return TS_TRUE;
}

TsBool				IHasTransform::SetWorldPos(const TsVector3&)
{

}
TsBool				IHasTransform::SetLocalPos(const TsVector3&)
{

}

TsBool				IHasTransform::SetLocalRotate(const TsQuaternion&)
{

}

TsBool				IHasTransform::SetWorldScale(const TsVector3&)
{

}
TsBool				IHasTransform::SetLocalScale(const TsVector3&)
{

}

TsBool				IHasTransform::SetLcaoRotateAxis(const TsVector3&, TsF32 angle)
{

}