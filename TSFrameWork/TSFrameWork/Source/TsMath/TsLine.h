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
class TsLine :public TsCollider
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

    virtual TsCollider::eType GetType()const override
    {
        return TsCollider::Collider_Line;
    }
private:
    //=========================================================================
    //! propery
    //=========================================================================
    T m_begin;
    T m_end;
};

template<typename T>
class TsRay :public TsLine<T>
{
    virtual TsCollider::eType GetType()const override
    {
        return TsCollider::Collider_Ray;
    }
};

typedef TsLine<TsVector2> TsLine2D;
typedef TsLine<TsVector3> TsLine3D;
typedef TsLine<TsVector4> TsLine4D;

typedef TsRay<TsVector2> TsRay2D;
typedef TsRay<TsVector3> TsRay3D;
typedef TsRay<TsVector4> TsRay4D;

extern template class TsLine<TsVector2>;
extern template class TsLine<TsVector3>;
extern template class TsLine<TsVector4>;

extern template class TsRay<TsVector2>;
extern template class TsRay<TsVector3>;
extern template class TsRay<TsVector4>;