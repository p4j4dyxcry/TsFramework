#include "../../TsMath/TsTransForm.h"

#include "../TsUT.h"

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
	return  m_pTransform->GetWorldPos();
}
TsMatrix			IHasTransform::GetWorldMatrix()const
{
	return m_pTransform->ToWorldMatrix();
}

const TsTransForm*	IHasTransform::GetTransformPtr()const
{
	return m_pTransform;
}

TsBool IHasTransform::SetParent(IHasTransform* parent)
{
	m_pTransform->SetParent(parent->m_pTransform);
	return TS_TRUE;
}
TsBool	IHasTransform::AddChild(IHasTransform* child)
{
	m_pTransform->AddChild(child->m_pTransform);
	return TS_TRUE;
}

TsBool IHasTransform::SetWorldPos(const TsVector3& pos)
{
	TsVector3 parentWorldPos = m_pTransform->GetWorldPos() - m_pTransform->m_localPosition;
	m_pTransform->m_localPosition = pos - parentWorldPos;
	return TS_TRUE;

}
TsBool				IHasTransform::SetLocalPos(const TsVector3& pos)
{
	m_pTransform->m_localPosition = pos ;
}

TsBool				IHasTransform::SetLocalRotate(const TsQuaternion& q)
{
	m_pTransform->m_localRotate = q;
}

TsBool				IHasTransform::SetWorldScale(const TsVector3& s)
{
	TsVector3 scale = m_pTransform->GetWorldScale();
	if (scale == 0)
		return TS_FALSE;
	TsVector3 invScale = TsVector3::one / scale;
}
TsBool				IHasTransform::SetLocalScale(const TsVector3&)
{

}

TsBool				IHasTransform::SetLcaoRotateAxis(const TsVector3&, TsF32 angle)
{

}