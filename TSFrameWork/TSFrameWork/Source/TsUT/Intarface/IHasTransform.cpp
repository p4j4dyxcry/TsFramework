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

TsVector3 IHasTransform::GetLocalPosition()const
{
    return m_pTransform->m_localPosition;
}
TsQuaternion IHasTransform::GetLocalRotate()const
{
    return m_pTransform->m_localRotate;
}
TsVector3 IHasTransform::GetLocalScale()const
{
    return m_pTransform->m_localScale;
}

TsVector3 IHasTransform::GetWorldPosition()const
{
    return  m_pTransform->GetWorldPos();
}
TsMatrix IHasTransform::GetWorldMatrix()const
{
    return m_pTransform->ToWorldMatrix();
}

//! ワールド座標上でのオブジェクトの拡縮を取得
TsVector3 IHasTransform::GetWorldScale()const
{
    TsVector3 scale = m_pTransform->m_localScale;
    TsTransForm* pPointer = m_pTransform->GetParent();
    for( ; pPointer != nullptr;pPointer = pPointer->GetParent() )
        scale *= pPointer->m_localScale;
    return scale;
}

//! ワールド座標上での回転を取得
TsQuaternion IHasTransform::GetWorldRotate()const
{
    TsQuaternion q ;
    TsTransForm* ptr = m_pTransform;
    TsStack<TsQuaternion> rotateStack;

    for( ; ptr != nullptr; ptr = ptr->GetParent() )
        rotateStack.push( ptr->m_localRotate );

    for( ; !rotateStack.empty(); rotateStack.pop() )
        q *= rotateStack.top();

    return q;
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

//! 親を取得
TsTransForm* IHasTransform::GetParent()const
{
    return m_pTransform->GetParent();
}

//! 子を取得
TsTransForm* IHasTransform::FindChild( const TsString& name)const
{
    return m_pTransform->FindChildByhash( TSUT::StringToHash( name ) );
}

TsBool IHasTransform::SetWorldPosition(const TsVector3& pos)
{
    TsVector3 parentWorldPos = m_pTransform->GetWorldPos() - m_pTransform->m_localPosition;
    m_pTransform->m_localPosition = pos - parentWorldPos;
    return TS_TRUE;

}
TsBool IHasTransform::SetLocalPosition(const TsVector3& pos)
{
    m_pTransform->m_localPosition = pos ;
    return TS_TRUE;
}

TsBool IHasTransform::SetLocalRotate(const TsQuaternion& q)
{
    m_pTransform->m_localRotate = q;
    return TS_TRUE;
}

TsBool IHasTransform::SetWorldScale(const TsVector3& s)
{
    TsVector3 scale = m_pTransform->GetWorldScale();
    if (scale == 0)
        return TS_FALSE;
    TsVector3 invScale = TsVector3::one / scale;
    m_pTransform->m_localScale = invScale * s;

    return TS_TRUE;
}
TsBool IHasTransform::SetLocalScale(const TsVector3& scale)
{
    m_pTransform->m_localScale = scale;
    return TS_TRUE;
}

TsBool IHasTransform::SetLocalRotateAxis(const TsVector3& v, TsF32 angle)
{
    m_pTransform->m_localRotate = TsQuaternion::AngleAxis(v, angle);
    return TS_TRUE;
}