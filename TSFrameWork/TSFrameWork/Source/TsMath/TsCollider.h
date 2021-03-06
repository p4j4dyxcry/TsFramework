﻿//**********************************************************
//! TsCollider.h
//! コライダベース.
//!
//! © 2016 Yuki Tsuneyama

class TsCollider
{
public:
    enum eType
    {
        Collider_Line2D,
        Collider_Line3D,
        Collider_Ray2D,
        Collider_Ray3D,
        Collider_AABB2D,
        Collider_AABB3D,
        Collider_OBB3D,
        Collider_TsCircle,
        Collider_TsSphere,
        Collider_Capsule2D,
        Collider_Capsule3D,
        Collider_Unknown,
        Collider_Num,
    };

    virtual eType GetType()const
    {
        return Collider_Unknown;
    }
    virtual TsMatrix GetGeometoricMatrix()const
    {
        return TsMatrix::identity;
    }
};