//#include "../../TsAfx.h"

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
    TeLine<T> temp;
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
TsLine<T> TsLine<T>::Transform( const TsMatrix& )
{
    //todo ÇQéüå≥ópÅAÇRéüå≥ópÅ@ÇSéüå≥ópÇÃåvéZÇì¸ÇÍÇ»Ç¢Ç∆Ç¢ÇØÇ»Ç¢
    return *this;
}