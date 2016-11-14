
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
    *this = aabb;
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
    m_min *= matrix;
    m_max *= matrix;

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

