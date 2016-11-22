//!*******************************************************
//! TsSphere.h
//!
//! 球クラス.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

template< typename T>
class TsSphere
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

protected:
    //----------------------------------------------------------
    // peropery
    //----------------------------------------------------------
    T m_center;
    TsF32 m_radius;

};

typedef TsSphere<TsVector2> TsCircle;     //円
typedef TsSphere<TsVector2> TsSphere2D;   //円
typedef TsSphere<TsVector3> TsSphere3D;   //球

#include "TsSphere.cpp"