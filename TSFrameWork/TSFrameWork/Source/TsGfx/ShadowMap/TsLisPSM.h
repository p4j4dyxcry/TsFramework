#pragma once

class TsComputeLisPSM
{
private:

	class PointList
	{
	private:
		TsVector<TsVector3> m_list;
	public:
		PointList();
		TsInt GetSize()const;
		TsBool Add( TsVector3& value );
		TsBool Clear();
		TsBool Transform( const TsMatrix& matrix );
		TsBool ComputeAABB( TsVector3& min , TsVector3& max )const;

		PointList & operator = ( PointList& value );
		TsVector3& operator[] ( TsInt index );
	};

	PointList m_pointList;

	TsVector3 m_eyePostion;
	TsVector3 m_viewDir;
	TsVector3 m_lightDir;

	TsMatrix m_viewMatrix;
	TsMatrix m_viewProjectionMatrix;

	TsMatrix m_lightViewMatrix;
	TsMatrix m_lightProjectionMatrix;
	TsMatrix m_lVPMatrix;

	TsF32  m_near;

	TsBool ComputeLisPSM();
	TsBool ComputeShadowMap();
	TsBool ComputeViewFrustum( TsMatrix& viewProj );
	TsVector3 ComputeUpVector( TsVector3& viewDir , TsVector3 lightDir );
	TsMatrix GetUnitCubeClipMatrix( TsVector3& min , TsVector3& max );
	TsMatrix GetPerspective( TsF32 nearDist , TsF32 farDist );

public:
	TsComputeLisPSM();

	TsBool SetEyePos( TsVector3& value );
	TsBool SetViewDir( TsVector3& value );
	TsBool SetLightDir( TsVector3& value );
	TsBool SetEyeView( TsVector3& value );
	TsBool SetEyeProjection( TsMatrix& value );
	TsBool SetNearClip( TsF32 value );
	TsBool UpdateShadowMatrix();

	TsMatrix GetLVMatrix();
	TsMatrix GetLPMatrix();
	TsMatrix GetLVPMatrix();
};