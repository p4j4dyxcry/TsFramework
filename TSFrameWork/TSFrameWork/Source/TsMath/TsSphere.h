//!*******************************************************
//! TsSphere.h
//!
//! 球クラス.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

template< typename T>
class TsSphere :public TsCollider
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsSphere();
    TsSphere(const T& center , TsF32 r);
    TsSphere(const TsSphere<T>& s);
    virtual ~TsSphere();

    //----------------------------------------------------------
    //! operator
    //----------------------------------------------------------    
    TsSphere<T> operator = (const TsSphere<T>& s);

    TsBool Translate( const T& t );

    TsBool SetCenter( const T&  );
    TsBool SetRadius(const TsF32 r);

    T GetCenter()const;
    TsF32 GetRadius()const;

    virtual TsCollider::eType GetType()const override;

    virtual TsMatrix GetGeometoricMatrix()const override
    {
        TsVector3 scale = TsVector3::one * m_radius;
        TsVector3 translate = GetCenter();

        return TsMatrix::CreateScale(scale) * TsMatrix::CreateTranslate(translate);
    }

protected:
    //----------------------------------------------------------
    // peropery
    //----------------------------------------------------------
    T m_center;
    TsF32 m_radius;

};

extern template class TsSphere<TsVector2>;
extern template class TsSphere<TsVector3>;

typedef TsSphere<TsVector2> TsCircle;     //円
typedef TsSphere<TsVector2> TsSphere2D;   //円
typedef TsSphere<TsVector3> TsSphere3D;   //球