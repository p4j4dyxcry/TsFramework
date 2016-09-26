#pragma once

#include "DebugConsole.h"

#if	 (TSUNE_DEBUG)
#include "Math.h"
#include <cstdio>

	inline void DebugLog( const Vector2& v )
	{
		char buffer[255];
		sprintf_s( buffer , "%.2f,	%.2f\n" , v.x , v.y );
		DEBUG_PRINTF( buffer );
	}

	inline void DebugLog( const Vector3& v )
	{
		char buffer[255];
		sprintf_s( buffer , "%.2f,	%.2f,	%.2f \n" , v.x , v.y,v.z );
		DEBUG_PRINTF( buffer );
	}

	inline void DebugLog( const Vector4& v )
	{
		char buffer[255];
		sprintf_s( buffer , "%.2f,	%.2f,%.2f,	%.2f\n" , v.x , v.y , v.z ,v.w);
		DEBUG_PRINTF( buffer );
	}

	inline void DebugLog( const Quaternion& v )
	{
		char buffer[255];
		sprintf_s( buffer , "%.2f,	%.2f,	%.2f,	%.2f\n" , v.x , v.y , v.z , v.w );
		DEBUG_PRINTF( buffer );
	}

	inline void DebugLog( const CTransForm& v )
	{
		DEBUG_PRINTF( "Translate	:" );
		DebugLog( v.m_localPosition );
		

		float rx , ry , rz;

		XMMATRIX mtx = v.m_localRotate.ToMatrix().ToXMMATRIX();

		float PI = 3.1415926535f;

		float radian = PI / 180.0f;

		float degree = 180.0f / PI;
		//�s�񂩂�I�C���[�p

		//ZXY��]�@XMMatrixRotationRollPitchYaw�ō쐬�����s��
		//mtx = XMMatrixRotationRollPitchYaw(rx,ry,rz);
		//mtx����I�C���[�p�����߂�
		//������ -��/2 < rx < +��/2
		rx = -asinf( XMVectorGetY( mtx.r[2] ) );
		ry = atan2f( XMVectorGetX( mtx.r[2] ) , XMVectorGetZ( mtx.r[2] ) );
		rz = atan2f( XMVectorGetY( mtx.r[0] ) , XMVectorGetY( mtx.r[1] ) );

		rx *= degree;
		ry *= degree;
		rz *= degree;

		DEBUG_PRINTF( "Rotate		:" );
		DebugLog( Vector3(rx,ry,rz) );

		// �N�H�[�^�j�I������I�C���[�p
		// �s��ɕϊ����ċ��߂�
		XMVECTOR quat = XMQuaternionIdentity();
		mtx = XMMatrixRotationQuaternion( quat );
		DEBUG_PRINTF( "Scale		:" );
		DebugLog( v.m_localScale );
	}

#else
#define DebugLog(...) 
#endif