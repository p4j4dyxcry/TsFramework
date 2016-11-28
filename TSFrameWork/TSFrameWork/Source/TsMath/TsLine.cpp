#include "../../TsAfx.h"

// 明示的テンプレートのインスタンス化
template class TsLine<TsVector2>;
template class TsLine<TsVector3>;
template class TsLine<TsVector4>;

template class TsRay<TsVector2>;
template class TsRay<TsVector3>;
template class TsRay<TsVector4>;

//! Constructor
template< typename T>
TsLine<T>::TsLine()
{

}

template< typename T>
TsLine<T>::TsLine( const T& begin ,
                   const T&  end )
{
    SetBegin( begin );
    SetEnd( end );
}

template< typename T>
TsLine<T>::TsLine( const TsLine<T>& line )
{
    *this = line;
}

//! Destructor
template< typename T>
TsLine<T>::~TsLine(){};

//! opeator
template< typename T>
TsLine<T> TsLine<T>::operator + ( const TsLine<T>& line )
{
    TsLine<T> temp = *this;
    temp.m_begin += line.m_begin;
    temp.m_end += line.m_end;

    return temp;
}

template< typename T>
TsLine<T> TsLine<T>::operator += ( const TsLine<T>& line )
{
    m_begin += line.m_begin;
    m_end += line.m_end;

    return *this;
}

template< typename T>
TsLine<T> TsLine<T>::operator - ( const TsLine<T>& line)
{
    TsLine<T> temp = *this;
    temp.m_begin -= line.m_begin;
    temp.m_end -= line.m_end;

    return temp;
}

template< typename T>
TsLine<T> TsLine<T>::operator -= ( const TsLine<T>& line )
{
    m_begin -= line.m_begin;
    m_end -= line.m_end;

    return *this;
}

template< typename T>
TsLine<T> TsLine<T>::operator = ( const TsLine<T>& line)
{
    m_begin = line.m_begin;
    m_end = line.m_end;

    return *this;
}

template< typename T>
TsLine<T> TsLine<T>::operator * ( const TsMatrix& m)
{
    TsLine<T> l = *this;
    l.Transform( m );
    return l;
}

template< typename T>
TsLine<T> TsLine<T>::operator *= ( const TsMatrix& m )
{
    return Transform(m);
}

template< typename T>
TsLine<T> TsLine<T>::operator * ( TsF32 f)
{
    TsLine<T> temp = *this;
    temp.m_end *= f;
    return temp;
}

template< typename T>
TsLine<T> TsLine<T>::operator *= ( TsF32 f)
{
    m_end *= f;
    return *this;
}

template< typename T>
TsLine<T> TsLine<T>::operator / ( TsF32 f)
{
    TsLine<T> temp = *this;
    temp.m_begin /= f;
    temp.m_end /= f;
    return temp;
}

template< typename T>
TsLine<T> TsLine<T>::operator /= ( TsF32 f)
{
    m_end /= f;
    return *this;
}

template< typename T>
TsLine<T> operator * ( TsF32 f ,const typename TsLine<T>& line )
{
    return line * f;
}

template< typename T>
TsLine<T> operator / ( TsF32 f , const typename TsLine<T>& line )
{
    return line / f;
}

template< typename T>
TsLine<T> TsLine<T>::operator - ( TsF32 )
{
    TsLine<T> temp;
    return temp * -1;
}

// method
template< typename T>
TsBool TsLine<T>::SetBegin( const T& v )
{
    m_begin = v;
    return TS_TRUE;
}

template< typename T>
TsBool TsLine<T>::SetEnd( const T& v )
{
    m_end = v;
    return TS_TRUE;
}

template< typename T>
const T& TsLine<T>::GetBegin()const
{
    return m_begin;
}

template< typename T>
const T& TsLine<T>::GetEnd()const
{
    return m_end;
}

template< typename T>
T TsLine<T>::GetNormalizeVector()const
{
    return GetVector().Normalized();
}

template< typename T>
T TsLine<T>::GetVector()const
{
    return ( m_end - m_begin );
}

template< typename T>
TsF32 TsLine<T>::Length()const
{
    return GetVector().Length();
}

template< typename T>
TsLine<T> TsLine<T>::Transform( const TsMatrix& matrix)
{
    m_begin *= matrix;
    m_end   *= matrix;
    return *this;
}

template<>
TsCollider::eType TsLine2D::GetType()const
{
    return TsCollider::Collider_Line2D;
}

template<>
TsCollider::eType TsLine3D::GetType()const
{
    return TsCollider::Collider_Line3D;
}

template<typename T>
TsCollider::eType TsLine<T>::GetType()const
{
    return TsCollider::Collider_Unknown;
}

template<>
TsCollider::eType TsRay2D::GetType()const
{
    return TsCollider::Collider_Ray2D;
}

template<>
TsCollider::eType TsRay3D::GetType()const
{
    return TsCollider::Collider_Ray3D;
}

template<typename T>
TsCollider::eType TsRay<T>::GetType()const
{
    return TsCollider::Collider_Unknown;
}