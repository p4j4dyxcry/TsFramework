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
    vector[3] = m_min;  vector[3].z = m_max.z;

    vector[4] = m_max;
    vector[5] = m_max;   vector[5].x = m_min.x;
    vector[6] = m_max;   vector[6].y = m_min.y;
    vector[7] = m_max;   vector[7].z = m_min.z;

    return vector;
}

TsVector<TsLine2D> TsAABB<TsVector2>::GetEdgeList()const
{
    TsVector<TsLine2D> vector;
    auto&& pList = GetVertexList();

    vector.resize(4);
    vector[0].SetBegin(pList[0]);
    vector[0].SetEnd(pList[1]);

    vector[1].SetBegin(pList[1]);
    vector[1].SetEnd(pList[2]);

    vector[2].SetBegin(pList[2]);
    vector[2].SetEnd(pList[3]);

    vector[3].SetBegin(pList[3]);
    vector[3].SetEnd(pList[0]);

    return vector;
}

TsVector<TsLine3D> TsAABB<TsVector3>::GetEdgeList()const
{
    TsVector<TsLine3D> vector;
    auto&& pList = GetVertexList();

    vector.resize(6);

    vector[0].SetBegin(pList[0]);
    vector[0].SetEnd(pList[1]);

    vector[1].SetBegin(pList[0]);
    vector[1].SetEnd(pList[2]);

    vector[2].SetBegin(pList[0]);
    vector[2].SetEnd(pList[3]);


    vector[3].SetBegin(pList[7]);
    vector[3].SetEnd(pList[6]);

    vector[4].SetBegin(pList[7]);
    vector[4].SetEnd(pList[5]);

    vector[5].SetBegin(pList[7]);
    vector[5].SetEnd(pList[4]);

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