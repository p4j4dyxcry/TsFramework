//**********************************************************
//! TsAABB.h
//! Bounding Box.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

//=========================================================================
//! TsAABB 
//=========================================================================
template< typename T>
class TsAABB
{
public:
    //=========================================================================
    //! public method
    //=========================================================================

    //! Constructor
    TsAABB();
    TsAABB( const TsAABB<T>& );
    TsAABB( const T& min ,
            const T& max );


    //! Destructor
    virtual ~TsAABB();

    TsBool SetMin( const T& );
    TsBool SetMax( const T& );
    TsAABB<T> Transform( const TsMatrix& );
    const T& GetMin()const;
    const T& GetMax()const;

    //! operator
    TsAABB<T> operator * (const TsMatrix& matrix)const;
    TsAABB<T> operator *= (const TsMatrix& matrix);
    TsAABB<T> operator = ( const TsAABB<T>& aabb );

private:
    //=========================================================================
    //! propery
    //=========================================================================
    T m_min;
    T m_max;
};

typedef TsAABB<TsVector2> TsAABB2D;
typedef TsAABB<TsVector3> TsAABB3D;

#include "TsAABB.cpp"