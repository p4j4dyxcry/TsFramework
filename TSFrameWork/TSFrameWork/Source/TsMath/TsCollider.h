//**********************************************************
//! TsCollider.h
//! コライダベース.
//!
//! © 2016 Yuki Tsuneyama

class TsCollider
{
public:
    enum eType
    {
        Collider_Line,
        Collider_Ray,
        Collider_AABB,
        Collider_OBB,
        Collider_Unknown,
    };

    virtual eType GetType()const
    {
        return Collider_Unknown;
    }
};