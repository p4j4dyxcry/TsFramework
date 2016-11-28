//!*******************************************************
//! TsTriangle.h
//!
//! 三角形.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

template <typename T>
class TsTriangle : TsCollider
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsTriangle(){};
    virtual ~TsTriangle(){};

    TsBool SetVertex(TsUint index, const T& vertex);

    const T& GetVertex(TsUint index)const;
    T GetVertex(TsUint index);

    T& operator [](TsUint index);
    const T& operator [](TsUint index)const;

    // 重心を計算する
    T ComputeCenterOfGravity()const;

    // 境界球(円)を計算する
    TsSphere<T> ComputeBoundingSphere()const;

    // 境界ボックスを計算する
    TsAABB<T>   ComputeAABB()const;
protected:
    //----------------------------------------------------------
    // peropery
    //----------------------------------------------------------
    T m_vertex[3];
};

extern template class TsTriangle<TsVector2>;
extern template class TsTriangle<TsVector3>;

typedef TsTriangle<TsVector2> TsTriangle2D;
typedef TsTriangle<TsVector3> TsTriangle3D;