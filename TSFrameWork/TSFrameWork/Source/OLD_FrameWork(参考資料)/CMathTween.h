//!*******************************************************
//!	CMathTween.h
//!
//!	ï‚äÆÉNÉâÉX
//!
//!	íòçÏå†	2016 Yuki Tsuneyama
//!	çÏê¨ì˙éû	16.01.13
#pragma once

#include "CVector.h"
#include "CQuaternion.h"

class CMathTween
{
public:
	static const enum class LerpType
	{
		Linear,
		Cubic,
		EaseIn,
		EaseOut,
		Spring,
	};

	static	FLOAT	Lerp( FLOAT x0 , FLOAT x1 , FLOAT t , LerpType type = LerpType::Linear);
	static 	Vector2	Lerp( const Vector2& v0 , const Vector2& v1 , FLOAT t , LerpType type = LerpType::Linear );
	static 	Vector3	Lerp( const Vector3& v0 , const Vector3& v1 , FLOAT t , LerpType type = LerpType::Linear );
	static 	Vector4	Lerp( const Vector4& v0 , const Vector4& v1 , FLOAT t , LerpType type = LerpType::Linear );
	static	Quaternion Slerp( const Quaternion& q0 , const Quaternion& q1 , FLOAT t , LerpType type = LerpType::Linear );
};