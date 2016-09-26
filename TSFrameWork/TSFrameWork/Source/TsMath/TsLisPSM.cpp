#include "TsMath.h"

TsComputeLisPSM::PointList::PointList()
{
	m_list.reserve( 128 );
}

TsInt TsComputeLisPSM::PointList::GetSize()const
{
	return (TsInt)m_list.size();
};
TsBool TsComputeLisPSM::PointList::Add( TsVector3& value )
{
	m_list.push_back( value );
	return TS_TRUE;
}
TsBool TsComputeLisPSM::PointList::Clear()
{
	m_list.clear();
	return TS_TRUE;
}
TsBool TsComputeLisPSM::PointList::Transform( const TsMatrix & matrix )
{
	for each( auto var in  m_list )
		var.TransformCood( matrix );
	return TS_TRUE;
}
TsBool TsComputeLisPSM::PointList::ComputeAABB( TsVector3& min , TsVector3 & max )const
{
	if( GetSize() > 0 )
	{
		for each ( auto var in m_list )
		{
			min = TsMin( min , var );
			max = TsMin( max , var );
		}
	}
	else
	{
		return TS_FALSE;
	}
	return TS_TRUE;
}

TsComputeLisPSM::PointList& TsComputeLisPSM::PointList::operator =( PointList& value )
{
	m_list.clear();
	for( TsInt i = 0; i < value.GetSize(); ++i )
	{
		Add( value[i] );
	}
	return ( *this );
}

TsVector3& TsComputeLisPSM::PointList::operator[]( TsInt index )
{
	return m_list[index];
}
TsComputeLisPSM::TsComputeLisPSM()
{
	m_near = 0.1f;
	m_pointList.Clear();
}
TsVector3 TsComputeLisPSM::ComputeUpVector( TsVector3& viewDir , TsVector3 lightDir )
{
	TsVector3 left = TsVector3::Cross( lightDir , viewDir );
	TsVector3 up = TsVector3::Cross( left , lightDir );
	return up.Normalized();
}
TsBool TsComputeLisPSM::ComputeShadowMap()
{
	TsVector3 max , min;
	TsMatrix lightView = 
		TsMatrix::CreateLookAt( m_eyePostion ,
								m_eyePostion + m_lightDir ,
								m_viewDir );
	m_pointList.Transform( lightView );

	m_pointList.ComputeAABB( min , max );

	m_lightProjectionMatrix = GetUnitCubeClipMatrix( min , max );

	return TS_TRUE;
}

TsBool TsComputeLisPSM::ComputeLisPSM()
{
	TsVector3 max , min;

	TsF32 angle = TsVector3::CrossingAngle( m_viewDir , m_lightDir );

	//失敗時は通常のシャドウマップを算出
	if( angle == 0 || angle == TS_PI )
	{
		ComputeShadowMap();
		return TS_FALSE;
	}

	PointList listCloner = m_pointList;

	TsF32 singGamma = sqrtf( 1.0f - angle * angle );

	TsVector3 up = ComputeUpVector( m_viewDir , m_lightDir );

	m_lightViewMatrix = TsMatrix::CreateLookAt( m_eyePostion ,
												m_eyePostion + m_lightDir ,
												up );

	m_pointList.Transform( m_lightViewMatrix );
	m_pointList.ComputeAABB( min , max );

	//新しいフラスタムを求める
	const TsF32 factor = 1.0f / singGamma;
	const TsF32 z_n = factor * m_near;
	const TsF32 d = abs( max.y - min.y );

	const TsF32 z0 = -z_n;
	const TsF32 z1 = -( z_n + d * singGamma );
	const TsF32 n = d / ( sqrtf( z1 / z0 ) - 1.0f );

	const TsF32 f = n + d;

	TsVector3 pos = m_eyePostion - up * ( n - m_near );

	m_lightViewMatrix = TsMatrix::CreateLookAt( pos , ( pos + m_lightDir ) , up );
	TsMatrix proj = GetPerspective( n , f );

	m_lightProjectionMatrix = m_lightViewMatrix * proj;
	listCloner.Transform( m_lightProjectionMatrix );
	listCloner.ComputeAABB( min , max );

	TsMatrix clip = GetUnitCubeClipMatrix( min , max);

	m_lightProjectionMatrix = proj * clip;

	return TS_TRUE;
}

TsBool TsComputeLisPSM::UpdateShadowMatrix()
{
	TsMatrix viewProj;
	viewProj = m_viewMatrix * m_viewProjectionMatrix;

	//ビューボリュームを求める
	ComputeViewFrustum( viewProj );

	// Light Space Perspective Shadow Map
	ComputeLisPSM();

	//左手座標系に変換
	//TsMatrix scale = TsMatrix::CreateScale( 1.0f , 1.0f , -1.0f );
	//m_lightProjectionMatrix = m_lightProjectionMatrix * scale;
	m_lVPMatrix = m_lightViewMatrix * m_lightProjectionMatrix;

	return TS_TRUE;
}
TsBool TsComputeLisPSM::ComputeViewFrustum( TsMatrix& viewProj )
{
	m_pointList.Clear();

	//　立方体を作成
	TsVector3 v[8];
	v[0] = TsVector3( -1.0f , +1.0f , -1.0f );
	v[1] = TsVector3( -1.0f , -1.0f , -1.0f );
	v[2] = TsVector3( +1.0f , -1.0f , -1.0f );
	v[3] = TsVector3( +1.0f , +1.0f , -1.0f );
	v[4] = TsVector3( -1.0f , +1.0f , +1.0f );
	v[5] = TsVector3( -1.0f , -1.0f , +1.0f );
	v[6] = TsVector3( +1.0f , -1.0f , +1.0f );
	v[7] = TsVector3( +1.0f , +1.0f , +1.0f );
	for( int i = 0; i<8; i++ )
	{
		m_pointList.Add( v[i] );
	}

	//　ビュー行列→射影行列の逆変換を行う行列を求める
	TsMatrix invViewProj = viewProj.Inversed();

	//　立方体に逆変換する行列をかけ、視錐台を求める
	m_pointList.Transform( invViewProj );

	return TS_TRUE;
}

TsMatrix TsComputeLisPSM::GetUnitCubeClipMatrix( TsVector3& min , TsVector3& max )
{
	TsMatrix result;

	result._11 = 2.0f / ( max.x - min.x );
	result._21 = 0.0f;
	result._31 = 0.0f;
	result._41 = -( max.x + min.x ) / ( max.x - min.x );

	result._12 = 0.0f;
	result._22 = 2.0f / ( max.y - min.y );
	result._32 = 0.0f;
	result._42 = -( max.y + min.y ) / ( max.y - min.y );

	result._13 = 0.0f;
	result._23 = 0.0f;
	result._33 = 1.0f / ( max.z - min.z );
	result._43 = -min.z / ( max.z - min.z );

	result._14 = 0.0f;
	result._24 = 0.0f;
	result._34 = 0.0f;
	result._44 = 1.0f;

	return result;
}
TsMatrix TsComputeLisPSM::GetPerspective( TsF32 nearDist , TsF32 farDist )
{
	TsMatrix result;
	result = TsMatrix::identity;
	result._22 = farDist / ( farDist - nearDist );
	result._24 = 1.0f;
	result._42 = -farDist * nearDist / ( farDist - nearDist );
	result._44 = 0.0f;
	return result;
}

TsBool TsComputeLisPSM::SetEyePos( TsVector3& value )
{
	m_eyePostion = value;
	return TS_TRUE;
}
TsBool TsComputeLisPSM::SetViewDir( TsVector3& value )
{
	m_viewDir = value;
	return TS_TRUE;
}
TsBool TsComputeLisPSM::SetLightDir( TsVector3& value )
{
	m_lightDir = value;
	return TS_TRUE;
}
TsBool TsComputeLisPSM::SetEyeView( TsVector3& value )
{
	m_eyePostion = value;
	return TS_TRUE;
}
TsBool TsComputeLisPSM::SetEyeProjection( TsMatrix& value )
{
	m_viewProjectionMatrix = value;
	return TS_TRUE;
}
TsBool TsComputeLisPSM::SetNearClip( TsF32 value )
{
	if( value <= 0 )
		return TS_FALSE;
	m_near = value;
	return TS_TRUE;
}

TsMatrix TsComputeLisPSM::GetLVMatrix()
{
	return m_lightViewMatrix;
}
TsMatrix TsComputeLisPSM::GetLPMatrix()
{
	return m_lightProjectionMatrix;
}
TsMatrix TsComputeLisPSM::GetLVPMatrix()
{
	return m_lVPMatrix;
}