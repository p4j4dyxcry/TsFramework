#include "../../TsAfx.h"

template class TsTriangle<TsVector2>;
template class TsTriangle<TsVector3>;

template<typename T>
TsBool TsTriangle<T>::SetVertex(TsUint index, const T& vertex)
{
    (*this)[index] = vertex;
    return TS_TRUE;
}

template<typename T>
const T& TsTriangle<T>::GetVertex(TsUint index)const
{
    return m_vertex[index];
}

template<typename T>
T TsTriangle<T>::GetVertex(TsUint index)
{
    return m_vertex[index];
}

template<typename T>
T& TsTriangle<T>::operator [](TsUint index)
{
    return m_vertex[index];
}

template<typename T>
const T& TsTriangle<T>::operator [](TsUint index)const
{
    return m_vertex[index];
}

// 重心を計算する
template<typename T>
T TsTriangle<T>::ComputeCenterOfGravity()const
{
    T result;
    TsUint sz = sizeof(T) / sizeof(TsF32);
    for (TsUint i = 0; i < sz; ++i)
    {
        result[i] = m_vertex[0][i] + 
                    m_vertex[1][i] + 
                    m_vertex[2][i] ;
    }
    return result;
}

// 境界球(円)を計算する
template<typename T>
TsSphere<T> TsTriangle<T>::ComputeBoundingSphere()const
{
    TsSphere<T> result;

    //重心
    T&& centerOfGravity = ComputeCenterOfGravity();

    //最も離れている位置を計算する
    TsF32 length = 0;
    for (TsUint i = 1; i < 3; ++i)
    {
        length = TsMax(length, (centerOfGravity - m_vertex[i]).Length());
    }
    result.SetCenter(centerOfGravity);
    result.SetRadius(length);

    return result;
}

// 境界ボックスを計算する
template<typename T>
TsAABB<T> TsTriangle<T>::ComputeAABB()const
{
    TsAABB<T> result;

    T min = TsVector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    T max = TsVector3( FLT_MAX,  FLT_MAX,  FLT_MAX);

    for (TsInt i = 0; i < 3; ++i)
    {
        min = TsMin(min, m_vertex[i]);
        max = TsMax(max, m_vertex[i]);
    }

    result.SetMin(min);
    result.SetMax(max);

    return result;
}
