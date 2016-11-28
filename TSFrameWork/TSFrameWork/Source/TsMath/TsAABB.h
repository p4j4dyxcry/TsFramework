﻿//**********************************************************
//! TsAABB.h
//! Bounding Box.
//!
//! © 2016 Yuki Tsuneyama

#pragma once

//=========================================================================
//! TsAABB 
//=========================================================================
template< typename T>
class TsAABB : public TsCollider
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
    TsInt    GetVertexSize()const;
    TsVector<T> GetVertexList()const;


    //! operator
    TsAABB<T> operator * (const TsMatrix& matrix)const;
    TsAABB<T> operator *= (const TsMatrix& matrix);
    TsAABB<T> operator = ( const TsAABB<T>& aabb );

    virtual eType GetType()const override;
private:
    //=========================================================================
    //! propery
    //=========================================================================
    T m_min;
    T m_max;
};

typedef TsAABB<TsVector2> TsAABB2D;
typedef TsAABB<TsVector3> TsAABB3D;

extern template class TsAABB<TsVector2>;
extern template class TsAABB<TsVector3>;