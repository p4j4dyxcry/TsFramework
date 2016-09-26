#pragma once

#define TS_PI (3.14159265359f)

inline TsF32 TsRadian( TsF32 degree )
{
	return degree *	180 / TS_PI;;
}

inline TsF32 TsDegree( TsF32 radian )
{
	return radian * TS_PI / 180.0f ;;
}

template<typename T>
inline T TsClamp( const T& value , const T& min , const T& max )
{
	return value < min ? min : value > max ? max : value;
}

template<typename T>
inline T TsMax( const T& a , const T& b )
{
	return a > b ? a : b;
}

template<typename T>
inline T TsMin( const T& a , const T& b )
{
	return a < b ? a : b;
}

template<typename T>
inline T TsLerp( const T& a , const T&b , TsF32 t )
{
	return a * ( 1 - t ) + b * t;
}

template<typename T>
inline T TsQubic( const T& a , const T&b , TsF32 t )
{
	return TsLerp( a , b , t * t * ( 3.0f - 2.0f * t ) );
}
//OŠpŒ`‚Ì–@ü‚ğŒvZ
inline TsVector3 TsComputeFaceNormal( TsVector3 v0 , TsVector3 v1 , TsVector3 v2 )
{
	TsVector3 n = TsVector3::Cross( ( v0 - v1 ) , ( v1 - v2 ) );
	return n.Normalized();
}
//OŠpŒ`‚ÌŠp“xiŒX‚«)‚ğŒvZ
inline TsF32 TsComputeFaceAngle( TsVector3 v0 , TsVector3 v1 , TsVector3 v2 )
{
	TsF32 angle = TsVector3::Dot( ( v0 - v1 ) , ( v1 - v2 ) );
	return acos( angle );
}
//OŠpŒ`‚Ì–ÊÏ‚ğŒvZ
inline TsF32 TsComputeTriangleArea( TsVector3 v0 , TsVector3 v1 , TsVector3 v2 )
{
	TsVector3 cross = TsVector3::Cross( ( v0 - v1 ) , ( v1 - v2 ) );
	
	return cross.Length() / 2;
}
