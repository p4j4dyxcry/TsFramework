//!*******************************************************
//! TsAnimationKey.h
//!
//! AnimationKey.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

template <typename T>
class TsAnimationKey : public TsNameObject
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsAnimationKey();
    virtual ~TsAnimationKey();

    const TsF32& time()const
    {
        return m_keyTime;
    }

    const T& Value()const
    {
        return m_keyVelue;
    }


protected:
    //----------------------------------------------------------
    // peropery
    //----------------------------------------------------------
    TsF32 m_keyTime;
    T     m_keyVelue;

    TsAnimationKey<T>* m_leftKey;
    TsAnimationKey<T>* m_rightKey;
    TsF32 m_leftTangent;
    TsF32 m_rightTangent;
};