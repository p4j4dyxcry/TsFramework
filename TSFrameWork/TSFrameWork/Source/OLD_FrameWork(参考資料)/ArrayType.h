//!*******************************************************
//!	ArrayType.h
//!
//!	Int型のベクトルクラスなどを使用可能にしています。
//!
//!	著作権	2016 Yuki Tsuneyama
//!	作成日時	16.01.18
#pragma once

//!*******************************************************
//!	[クラス] ２つのデータを持つクラス
//!
template<typename Type>
class CData2
{
public:
	union
	{
		Type m[2];
		struct
		{
			Type x , y;
		};
	};

	CData2( Type _x = 0 , Type _y = 0 )
	{
		x = ( Type )_x;
		y = ( Type )_y;
	}
	template<typename T>
	CData2( T* _array )
	{
		x = ( Type )_array[0];
		y = ( Type )_array[1];
	}
	template<typename T>
	CData2( const T & v )
	{
		x = ( Type )v.x;
		y = ( Type )v.y;
	}

	inline Type& operator[]( int n )
	{
		return m[n];
	}

	template<typename T>
	inline CData2<Type>& operator = ( const T& v )
	{
		x = ( Type )v.x;
		y = ( Type )v.y;
		return *this:
	}
	template<typename T>
	inline CData2<Type> operator+ ( const T& n )
	{
		return CData2<Type>( x + ( Type )n.x , y + ( Type )n.y );
	}
	template<typename T>
	inline CData2<Type> operator- ( const T& n )
	{
		return CData2<Type>( x - ( Type )n.x , y - ( Type )n.y );
	}
	template<typename T>
	inline CData2<Type> operator* ( const T& n )
	{
		return CData2( x*( Type )n.x , y * ( Type )n.y );
	}

	inline CData2<Type> operator* ( Type n )
	{
		return CData2( x*( Type )n , y*( Type )n );
	}
	template<typename T>
	inline CData2<Type> operator/ ( const T& n )
	{
		return CData2( x / ( Type )n.x , y / ( Type )n.y );
	}

	inline CData2<Type> operator/ ( Type n )
	{
		return CData2( x / ( Type )n , y / ( Type )n );
	}

	template<typename T>
	inline CData2<Type>& operator+= ( const T& n )
	{
		x += ( Type )n.x;
		y += ( Type )n.y;
		return *this;
	}
	template<typename T>
	inline CData2<Type>& operator-= ( const T& n )
	{
		x -= ( Type )n.x;
		y -= ( Type )n.y;
		return *this;
	}
	template<typename T>
	inline CData2<Type>& operator*= ( const T& n )
	{
		x *= ( Type )n.x;
		y *= ( Type )n.y;
		return *this;
	}

	inline CData2<Type>& operator*= ( Type n )
	{
		x *= ( Type )n;
		y *= ( Type )n;
		return *this;
	}
	template<typename T>
	inline CData2<Type>& operator/= ( const T& n )
	{
		x /= ( Type )n.x;
		y /= ( Type )n.y;
		return *this;
	}

	inline CData2<Type>& operator/= ( Type n )
	{
		x /= ( Type )n;
		y /= ( Type )n;
		return *this;
	}

	inline Type	 Sum()
	{
		return x + y;
	}
};


//!*******************************************************
//!	[クラス] 3つのデータを持つクラス
//!
template<typename Type>
class CData3
{
public:
	union
	{
		Type m[3];
		struct
		{
			Type x , y , z;
		};
	};

	CData3( Type _x = 0 ,
			Type _y = 0 ,
			Type _z = 0 )
	{
		x = ( Type )_x;
		y = ( Type )_y;
		z = ( Type )_z;
	}

	CData3( CData2<Type> n , Type _z )
	{
		x = n.x;
		y = n.y;
		z = _z;
	}

	template<typename T>
	CData3( T* _array )
	{
		x = ( Type )_array[0];
		y = ( Type )_array[1];
		z = ( Type )_array[2];
	}
	template<typename T>
	CData3( const T & n )
	{
		*this = n;
	}

	inline Type& operator[]( int n )
	{
		return m[n];
	}

	template<typename T>
	inline CData3<Type>& operator = ( const T& n )
	{
		x = ( Type )n.x;
		y = ( Type )n.y;
		z = ( Type )n.z;
		return *this;
	}
	template<typename T>
	inline CData3<Type> operator+ ( const T& n )
	{
		return CData3<Type>( x + ( Type )n.x , y + ( Type )n.y , z + ( Type )n.z );
	}
	template<typename T>
	inline CData3<Type> operator- ( const T& n )
	{
		return CData3<Type>( x - ( Type )n.x , y - ( Type )n.y , z - ( Type )n.z );
	}
	template<typename T>
	inline CData3<Type> operator* ( const T& n )
	{
		return CData3( x*( Type )n.x , y * ( Type )n.y , z*( Type )n.z );
	}

	inline CData3<Type> operator* ( Type n )
	{
		return CData3( x*( Type )n , y*( Type )n , z*( Type )n );
	}
	template<typename T>
	inline CData3<Type> operator/ ( const T& n )
	{
		return CData3( x / ( Type )n.x , y / ( Type )n.y , z / ( Type )n.z );
	}

	inline CData3<Type> operator/ ( Type n )
	{
		return CData3( x / ( Type )n , y / ( Type )n , z / ( Type )n );
	}

	template<typename T>
	inline CData3<Type>& operator+= ( const T& n )
	{
		x += ( Type )n.x;
		y += ( Type )n.y;
		z += ( Type )n.z;
		return *this;
	}
	template<typename T>
	inline CData3<Type>& operator-= ( const T& n )
	{
		x -= ( Type )n.x;
		y -= ( Type )n.y;
		z -= ( Type )n.z;
		return *this;
	}
	template<typename T>
	inline CData3<Type>& operator*= ( const T& n )
	{
		x *= ( Type )n.x;
		y *= ( Type )n.y;
		z *= ( Type )n.z;
		return *this;
	}

	inline CData3<Type>& operator*= ( Type n )
	{
		x *= ( Type )n;
		y *= ( Type )n;
		z *= ( Type )n;
		return *this;
	}
	template<typename T>
	inline CData3<Type>& operator/= ( const T& n )
	{
		x /= ( Type )n.x;
		y /= ( Type )n.y;
		z /= ( Type )n.z;
		return *this;
	}

	inline CData3<Type>& operator/= ( Type n )
	{
		x /= ( Type )n;
		y /= ( Type )n;
		z /= ( Type )n;
		return *this;
	}

	inline Type	 Sum()
	{
		return x + y + z;
	}
};

//!*******************************************************
//!	[クラス] 4つのデータを持つクラス
//!
template<typename Type>
class CData4
{
public:
	union
	{
		Type m[4];
		struct
		{
			Type x , y , z , w;
		};
	};

	CData4( Type _x = 0 ,
			Type _y = 0 ,
			Type _z = 0 ,
			Type _w = 0 )
	{
		x = ( Type )_x;
		y = ( Type )_y;
		z = ( Type )_z;
		w = ( Type )_w;
	}

	CData4( CData3<Type> n , Type _z , Type _w )
	{
		x = n.x;
		y = n.y;
		z = n.w;
		w = _w;
	}

	CData4( CData2<Type> n , Type _z , Type _w )
	{
		x = n.x;
		y = n.y;
		z = _z;
		w = _w;
	}

	template<typename T>
	CData4( T* _array )
	{
		x = ( Type )_array[0];
		y = ( Type )_array[1];
		z = ( Type )_array[2];
		w = ( Type )_array[3];
	}
	template<typename T>
	CData4( const T & n )
	{
		*this = n;
	}

	inline Type& operator[]( int n )
	{
		return m[n];
	}

	template<typename T>
	inline CData4<Type>& operator = ( const T& n )
	{
		x = ( Type )n.x;
		y = ( Type )n.y;
		z = ( Type )n.z;
		w = ( Type )n.z;
		return *this;
	}
	template<typename T>
	inline CData4<Type> operator+ ( const T& n )
	{
		return CData4<Type>( x + ( Type )n.x , y + ( Type )n.y , z + ( Type )n.z , w + ( Type )n.w );
	}
	template<typename T>
	inline CData4<Type> operator- ( const T& n )
	{
		return CData4<Type>( x - ( Type )n.x , y - ( Type )n.y , z - ( Type )n.z , w + ( Type )n.w );
	}
	template<typename T>
	inline CData4<Type> operator* ( const T& n )
	{
		return CData4( x*( Type )n.x , y * ( Type )n.y , z*( Type )n.z , w + ( Type )n.w );
	}

	inline CData4<Type> operator* ( Type n )
	{
		return CData4( x*( Type )n , y*( Type )n , z*( Type )n , w*( Type )n );
	}
	template<typename T>
	inline CData4<Type> operator/ ( const T& n )
	{
		return CData4( x / ( Type )n.x , y / ( Type )n.y , z / ( Type )n.z , w / ( Type )n.z );
	}

	inline CData4<Type> operator/ ( Type n )
	{
		return CData4( x / ( Type )n , y / ( Type )n , z / ( Type )n , w / ( Type )n );
	}

	template<typename T>
	inline CData4<Type>& operator+= ( const T& n )
	{
		x += ( Type )n.x;
		y += ( Type )n.y;
		z += ( Type )n.z;
		w += ( Type )n.w;
		return *this;
	}
	template<typename T>
	inline CData4<Type>& operator-= ( const T& n )
	{
		x -= ( Type )n.x;
		y -= ( Type )n.y;
		z -= ( Type )n.z;
		w -= ( Type )n.w;
		return *this;
	}
	template<typename T>
	inline CData4<Type>& operator*= ( const T& n )
	{
		x *= ( Type )n.x;
		y *= ( Type )n.y;
		z *= ( Type )n.z;
		w *= ( Type )n.w;
		return *this;
	}

	inline CData4<Type>& operator*= ( Type n )
	{
		x *= ( Type )n;
		y *= ( Type )n;
		z *= ( Type )n;
		w *= ( Type )n;
		return *this;
	}
	template<typename T>
	inline CData4<Type>& operator/= ( const T& n )
	{
		x /= ( Type )n.x;
		y /= ( Type )n.y;
		z /= ( Type )n.z;
		w /= ( Type )n.w;
		return *this;
	}

	inline CData4<Type>& operator/= ( Type n )
	{
		x /= ( Type )n;
		y /= ( Type )n;
		z /= ( Type )n;
		w /= ( Type )n;
		return *this;
	}

	inline Type	 Sum()
	{
		return x + y + z + w;
	}
};
typedef CData2<int>						Int2;
typedef CData2<unsigned int>			Uint2;
typedef CData2<short>					Short2;
typedef CData2<unsigned short>			UShort2;
typedef CData2<long long>				S64_2;
typedef CData2<unsigned long long>		U64_2;
typedef CData2<char>					S8_2;
typedef CData2<unsigned char>			U8_2;
typedef CData2<float>					Float2;

typedef CData3<int>						Int3;
typedef CData3<unsigned int>			Uint3;
typedef CData3<short>					Short3;
typedef CData3<unsigned short>			UShort3;
typedef CData3<long long>				S64_3;
typedef CData3<unsigned long long>		U64_3;
typedef CData3<char>					S8_3;
typedef CData3<unsigned char>			U8_3;
typedef CData3<float>					Float3;

typedef CData4<int>						Int4;
typedef CData4<unsigned int>			Uint4;
typedef CData4<short>					Short4;
typedef CData4<unsigned short>			UShort4;
typedef CData4<long long>				S64_4;
typedef CData4<unsigned long long>		U64_4;
typedef CData4<char>					S8_4;
typedef CData4<unsigned char>			U8_4;
typedef CData4<float>					Float4;