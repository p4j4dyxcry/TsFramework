//!*******************************************************
//! TsOBB.h
//!
//! Oriented Bouding Box.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsOBB : public TsCollider
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsOBB(const TsVector3&      center      = TsVector3::zero,
          const TsVector3&      scale       = TsVector3::one,
          const TsQuaternion&   quaternion  = TsQuaternion::identity);
    virtual ~TsOBB();

    const TsQuaternion& GetRotate()const;
    TsQuaternion& GetRotate();

    const TsVector3& GetCenter()const;
    TsVector3& GetCenter();

    const TsVector3& GetScale()const;
    TsVector3& GetScale();

    TsBool SetCenter(const TsVector3&   );
    TsBool SetScale (const TsVector3&   );
    TsBool SetRotate(const TsQuaternion&);

    virtual eType GetType()const override
    {
        return Collider_OBB3D;
    }
    virtual TsMatrix GetGeometoricMatrix()const override
    {
        TsVector3 scale = GetScale() * 2;
        TsVector3 translate = GetCenter();
        TsQuaternion rotate = GetRotate();

        return TsMatrix(translate, rotate, scale);
    }

protected:
    //----------------------------------------------------------
    // peropery
    //----------------------------------------------------------
    TsVector3 m_center;
    TsVector3 m_scale;
    TsQuaternion m_quaternion;
};