#include "../../TsAfx.h"

// 明示的テンプレートのインスタンス化
template class TsSphere<TsVector2>;
template class TsSphere<TsVector3>;


//----------------------------------------------------------
// Constructor
//----------------------------------------------------------
template<typename T>
TsSphere<T>::TsSphere()
:m_radius(0)
{
    
}

template<typename T>
TsSphere<T>::TsSphere(const T& center, TsF32 r)
{
    SetCenter(center);
    SetRadius(r);
}

template<typename T>
TsSphere<T>::TsSphere(const TsSphere<T>& s)
{
    *this = s;
}

template<typename T>
TsSphere<T>::~TsSphere()
{

}

//----------------------------------------------------------
//! operator
//---------------------------------------------------------- 
template<typename T>
TsSphere<T> TsSphere<T>::operator = (const TsSphere<T>& s)
{
    m_center = s.m_center;
    m_radius = s.m_radius;

    return *this;
}


//----------------------------------------------------------
//! public method
//---------------------------------------------------------- 
template<typename T>
TsBool TsSphere<T>::Translate(const T& t)
{
    m_center += t;

    return TS_TRUE;
}

template<typename T>
TsBool TsSphere<T>::SetCenter(const T& t)
{
    m_center = t;

    return TS_TRUE;
}

template<typename T>
TsBool TsSphere<T>::SetRadius(const TsF32 r)
{
    m_radius = r;

    return TS_TRUE;
}

template<typename T>
T TsSphere<T>::GetCenter()const
{
    return m_center;
}

template<typename T>
TsF32 TsSphere<T>::GetRadius()const
{
    return m_radius;
}

//template<typename T>
//TsCollider::eType TsSphere<T>::GetType()const
//{
//    return TsCollider::GetType();
//}

template<>
TsCollider::eType TsSphere<TsVector2>::GetType()const
{
    return TsCollider::Collider_TsCircle;
}

template<>
TsCollider::eType TsSphere<TsVector3>::GetType()const
{
    return TsCollider::Collider_TsSphere;
}
