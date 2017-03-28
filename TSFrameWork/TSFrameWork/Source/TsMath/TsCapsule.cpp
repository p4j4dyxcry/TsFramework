#include "../../TsAfx.h" 

// 明示的テンプレートのインスタンス化
template class TsCapsule<TsVector2>;
template class TsCapsule<TsVector3>;

template< typename T>
TsCapsule<T>::TsCapsule()
{

}

template< typename T>
TsCapsule<T>::TsCapsule( const T& bottom,
                         const T& up,
                         TsF32 radius)
{
    SetBottom(bottom);
    SetTop(up);
    SetRadius(radius);
}

template< typename T>
TsCapsule<T>::TsCapsule(const TsCapsule<T>& capsule)
{
    SetBottom(capsule.m_bottom);
    SetTop(capsule.m_top);
    SetRadius(capsule.m_radius);
}

template< typename T>
TsCapsule<T>::~TsCapsule()
{

}

template< typename T>
TsBool TsCapsule<T>::SetBottom(const T& bottom)
{
    m_bottom = bottom;
    return TS_TRUE;
}

template< typename T>
TsBool TsCapsule<T>::SetTop(const T& top)
{
    m_top = top;
    return TS_TRUE;
}

template< typename T>
TsBool TsCapsule<T>::SetRadius(TsF32 radius)
{
    m_radius = radius;
    return TS_TRUE;
}

template< typename T>
const T& TsCapsule<T>::GetBottom()const
{
    return m_bottom;
}

template< typename T>
const T& TsCapsule<T>::GetTop()const
{
    return m_top;
}

template< typename T>
TsF32 TsCapsule<T>::GetRadius()const
{
    return m_radius;
}
template< typename T>
T TsCapsule<T>::GetBottomToTopVector()const
{
    return m_bottom - m_top;
}


template<>
TsCollider::eType TsCapsule2D::GetType()const
{
    return TsCollider::Collider_AABB2D;
}

template<>
TsCollider::eType TsCapsule3D::GetType()const
{
    return TsCollider::Collider_AABB3D;
}

template<typename T>
TsCollider::eType TsCapsule<T>::GetType()const
{
    return TsCollider::Collider_Unknown;
}