#include "../../TsAfx.h"


// 明示的テンプレートのインスタンス化
template class TsAABB<TsVector2>;
template class TsAABB<TsVector3>;

template< typename T>
TsAABB<T>::TsAABB()
{

}

template< typename T>
TsAABB<T>::TsAABB(const T& min,
                  const T& max )
{
    SetMin( min );
    SetMax( max );
}

template< typename T>
TsAABB<T>::TsAABB(const TsAABB<T> & aabb)
{
    SetMin(aabb.m_min);
    SetMax(aabb.m_max);
}

template< typename T>
TsAABB<T>::~TsAABB()
{

}

template< typename T>
TsBool TsAABB<T>::SetMin(const T& v)
{
    m_min = v;
    return TS_TRUE;
}

template< typename T>
TsBool TsAABB<T>::SetMax(const T& v)
{
    m_max = v;
    return TS_TRUE;
}

template< typename T>
TsAABB<T> TsAABB<T>::Transform(const TsMatrix& matrix)
{
    T&& min = m_min * matrix;
    T&& max = m_max * matrix;

    m_min = TsMin<T>(min, max);
    m_max = TsMin<T>(min, max);

    return *this;
}

template< typename T>
const T& TsAABB<T>::GetMin()const
{
    return m_min;
}

template< typename T>
const T& TsAABB<T>::GetMax()const
{
    return m_max;
}

template< typename T>
T TsAABB<T>::GetCenter()const
{
    return (m_min + m_max)/2.0f;
}

template< typename T>
TsAABB<T> TsAABB<T>::operator * (const TsMatrix& matrix)const
{
    TsAABB aabb = *this;
    return aabb.Transform( matrix );
}

template< typename T>
TsAABB<T> TsAABB<T>::operator *= (const TsMatrix& matrix)
{
    return Transform(matrix);
}

template< typename T>
TsAABB<T> TsAABB<T>::operator = (const TsAABB<T>& aabb)
{
    SetMin( aabb.m_min );
    SetMax( aabb.m_max );
    return *this;
}
template< typename T>
TsInt    TsAABB<T>::GetVertexSize()const
{
    return sizeof(T) / sizeof(TsF32);
}
TsVector<TsVector2> TsAABB<TsVector2>::GetVertexList()const
{
    TsVector<TsVector2> vector;
    vector.resize(4);
    
    vector[0] = m_min;
    vector[1] = m_min;  vector[1].x = m_max.x;
    vector[2] = m_min;  vector[2].y = m_max.y;
    vector[3] = m_max;

    return vector;
}

TsVector<TsVector3> TsAABB<TsVector3>::GetVertexList()const
{
    TsVector<TsVector3> vector;
    vector.resize(8);

    vector[0] = m_min;
    vector[1] = m_min;  vector[1].x = m_max.x;
    vector[2] = m_min;  vector[2].y = m_max.y;
    vector[3] = m_max;

    vector[4] = vector[0] * TsVector3::back;
    vector[5] = vector[1] * TsVector3::back;
    vector[6] = vector[2] * TsVector3::back;
    vector[7] = vector[3] * TsVector3::back;

    return vector;
}

template<>
TsCollider::eType TsAABB2D::GetType()const
{
    return TsCollider::Collider_AABB2D;
}

template<>
TsCollider::eType TsAABB3D::GetType()const
{
    return TsCollider::Collider_AABB3D;
}

template<typename T>
TsCollider::eType TsAABB<T>::GetType()const
{
    return TsCollider::Collider_Unknown;
}