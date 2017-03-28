#pragma once

//=========================================================================
//! TsCapsule
//=========================================================================
template<typename T>
class TsCapsule :public TsCollider
{
public:
    //=========================================================================
    //! public method
    //=========================================================================

    //! Constructor
    TsCapsule<T>();
    TsCapsule(const T& bottom,
              const T& up,
              TsF32 radius);
    TsCapsule(const TsCapsule<T>&);

    //! Destructor
    virtual ~TsCapsule();

    // method
    TsBool SetBottom(const T& v);
    TsBool SetTop(const T& v);
    TsBool SetRadius(TsF32 radius);

    const T& GetBottom()const;
    const T& GetTop()const;
    TsF32 GetRadius()const;

    virtual TsCollider::eType GetType()const override;
private:
    //=========================================================================
    //! propery
    //=========================================================================
    T m_bottom;
    T m_top;
    TsF32 m_radius;
};

typedef TsCapsule<TsVector2> TsCapsule2D;
typedef TsCapsule<TsVector3> TsCapsule3D;