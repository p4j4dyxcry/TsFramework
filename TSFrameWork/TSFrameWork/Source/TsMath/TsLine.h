//**********************************************************
//! TsRay.h
//! 線分クラス.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

//=========================================================================
//! TsLine
//=========================================================================
template<typename T>
class TsLine
{
public:
    //=========================================================================
    //! public method
    //=========================================================================

    //! Constructor
    TsLine<T>();
    TsLine( const T& begin ,
            const T&  end );
    TsLine( const TsLine<T>& );

    //! Destructor
    virtual ~TsLine();

    //! opeator
    TsLine<T> operator + ( const TsLine<T>& );
    TsLine<T> operator - ( const TsLine<T>& );
    TsLine<T> operator += ( const TsLine<T>& );
    TsLine<T> operator -= ( const TsLine<T>& );

    TsLine<T> operator = ( const TsLine<T>& );

    TsLine<T> operator * ( const TsMatrix& );
    TsLine<T> operator *= ( const TsMatrix& );

    TsLine<T> operator * ( TsF32 );
    TsLine<T> operator / ( TsF32 );
    TsLine<T> operator *= ( TsF32 );
    TsLine<T> operator /= ( TsF32 );

    friend TsLine<T> operator * ( TsF32 ,const class TsLine<T>& );
    friend TsLine<T> operator / ( TsF32 , const class TsLine<T>& );
    TsLine<T> operator - ( TsF32 );

    // method
    TsBool SetBegin( const T& v );
    TsBool SetEnd( const T& v );

    const T& GetBegin()const;
    const T& GetEnd()const;

    T GetNormalizeVector()const;
    T GetVector()const;

    TsF32 Length()const;

    TsLine<T> Transform( const TsMatrix& );

private:
    //=========================================================================
    //! propery
    //=========================================================================
    T m_begin;
    T m_end;
};

typedef TsLine<TsVector2> TsLine2D;
typedef TsLine<TsVector3> TsLine3D;
typedef TsLine<TsVector4> TsLine4D;

#include "TsLine.cpp"