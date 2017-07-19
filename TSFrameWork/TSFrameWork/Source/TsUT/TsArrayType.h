//!*******************************************************
//! TsArrayType.h
//!
//! Int型のベクトルクラスなどを使用可能にしています。
//!
//! © 2016 Yuki Tsuneyama
#pragma once

template<typename Type>
class TsArrayType2
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

    TsArrayType2( Type _x = 0 , Type _y = 0 )
    {
        x = static_cast<Type>(_x);
        y = static_cast<Type>(_y);
    }
    template<typename T>
    TsArrayType2( T* _array )
    {
        x = static_cast<Type>(_array[0]);
        y = static_cast<Type>(_array[1]);
    }
    template<typename T>
    TsArrayType2( const T & v )
    {
        x = static_cast<Type>(v.x);
        y = static_cast<Type>(v.y);
    }

    inline const Type& operator[]( int n )const
    {
        return m[n];
    }

    inline Type& operator[]( int n )
    {
        return m[n];
    }

    template<typename T>
    inline TsArrayType2<Type>& operator = ( const T& v )
    {
        x = static_cast<Type>(v.x);
        y = static_cast<Type>(v.y);
        return *this;
    }
    template<typename T>
    inline TsArrayType2<Type> operator+ ( const T& n )const
    {
        return TsArrayType2<Type>( x + static_cast<Type>(n.x) , y + static_cast<Type>(n.y) );
    }
    template<typename T>
    inline TsArrayType2<Type> operator- ( const T& n )const
    {
        return TsArrayType2<Type>( x - static_cast<Type>(n.x) , y - static_cast<Type>(n.y) );
    }
    template<typename T>
    inline TsArrayType2<Type> operator* ( const T& n )const
    {
        return TsArrayType2( x*static_cast<Type>(n.x) , y * static_cast<Type>(n.y) );
    }

    inline TsArrayType2<Type> operator* ( Type n )const
    {
        return TsArrayType2( x*static_cast<Type>(n) , static_cast<Type>(n)*y );
    }
    template<typename T>
    inline TsArrayType2<Type> operator/ ( const T& n )const
    {
        return TsArrayType2( x / static_cast<Type>(n.x) , y / static_cast<Type>(n.y) );
    }

    inline TsArrayType2<Type> operator/ ( Type n )const
    {
        return TsArrayType2( x / static_cast<Type>(n) , y / static_cast<Type>(n) );
    }

    template<typename T>
    inline TsArrayType2<Type>& operator+= ( const T& n )
    {
        x += static_cast<Type>(n.x);
        y += static_cast<Type>(n.y);
        return *this;
    }
    template<typename T>
    inline TsArrayType2<Type>& operator-= ( const T& n )
    {
        x -= static_cast<Type>(n.x);
        y -= static_cast<Type>(n.y);
        return *this;
    }
    template<typename T>
    inline TsArrayType2<Type>& operator*= ( const T& n )
    {
        x *= static_cast<Type>(n.x);
        y *= static_cast<Type>(n.y);
        return *this;
    }

    inline TsArrayType2<Type>& operator*= ( Type n )
    {
        x *= static_cast<Type>(n);
        y *= static_cast<Type>(n);
        return *this;
    }
    template<typename T>
    inline TsArrayType2<Type>& operator/= ( const T& n )
    {
        x /= static_cast<Type>(n.x);
        y /= static_cast<Type>(n.y);
        return *this;
    }

    inline TsArrayType2<Type>& operator/= ( Type n )
    {
        x /= static_cast<Type>(n);
        y /= static_cast<Type>(n);
        return *this;
    }

    TsBool operator ==( const TsArrayType2<Type>& b )const
    {
        return x == b.x && y == b.y ;
    }

    TsBool operator !=( const TsArrayType2<Type>& b )const
    {
        return !(*this == b);
    }

    inline Type	 Sum()const
    {
        return x + y;
    }
};


//!*******************************************************
//!	[クラス] 3つのデータを持つクラス
//!
template<typename Type>
class TsArrayType3
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

    TsArrayType3( Type _x = 0 ,
            Type _y = 0 ,
            Type _z = 0 )
    {
        x = static_cast<Type>(_x);
        y = static_cast<Type>(_y);
        z = static_cast<Type>(_z);
    }

    TsArrayType3( TsArrayType2<Type> n , Type _z )
    {
        x = n.x;
        y = n.y;
        z = _z;
    }

    template<typename T>
    TsArrayType3( T* _array )
    {
        x = static_cast<Type>(_array[0]);
        y = static_cast<Type>(_array[1]);
        z = static_cast<Type>(_array[2]);
    }
    template<typename T>
    TsArrayType3( const T & n )
    {
        *this = n;
    }

    inline const Type& operator[]( int n )const
    {
        return m[n];
    }

    inline Type& operator[]( int n )
    {
        return m[n];
    }

    template<typename T>
    inline TsArrayType3<Type>& operator = ( const T& n )
    {
        x = static_cast<Type>(n.x);
        y = static_cast<Type>(n.y);
        z = static_cast<Type>(n.z);
        return *this;
    }
    template<typename T>
    inline TsArrayType3<Type> operator+ ( const T& n )const
    {
        return TsArrayType3<Type>( x + static_cast<Type>(n.x) , y + static_cast<Type>(n.y) , z + static_cast<Type>(n.z) );
    }
    template<typename T>
    inline TsArrayType3<Type> operator- ( const T& n )const
    {
        return TsArrayType3<Type>( x - static_cast<Type>(n.x) , y - static_cast<Type>(n.y) , z - static_cast<Type>(n.z) );
    }
    template<typename T>
    inline TsArrayType3<Type> operator* ( const T& n )const
    {
        return TsArrayType3( static_cast<Type>(n.x)*x , static_cast<Type>(n.y) * y , static_cast<Type>(n.z)*z );
    }

    inline TsArrayType3<Type> operator* ( Type n )const
    {
        return TsArrayType3( x*static_cast<Type>(n) , y*static_cast<Type>(n) , z*static_cast<Type>(n) );
    }
    template<typename T>
    inline TsArrayType3<Type> operator/ ( const T& n )const
    {
        return TsArrayType3( x / static_cast<Type>(n.x) , y / static_cast<Type>(n.y) , z / static_cast<Type>(n.z) );
    }

    inline TsArrayType3<Type> operator/ ( Type n )const
    {
        return TsArrayType3( x / static_cast<Type>(n) , y / static_cast<Type>(n) , z / static_cast<Type>(n) );
    }

    template<typename T>
    inline TsArrayType3<Type>& operator+= ( const T& n )
    {
        x += static_cast<Type>(n.x);
        y += static_cast<Type>(n.y);
        z += static_cast<Type>(n.z);
        return *this;
    }
    template<typename T>
    inline TsArrayType3<Type>& operator-= ( const T& n )
    {
        x -= static_cast<Type>(n.x);
        y -= static_cast<Type>(n.y);
        z -= static_cast<Type>(n.z);
        return *this;
    }
    template<typename T>
    inline TsArrayType3<Type>& operator*= ( const T& n )
    {
        x *= static_cast<Type>(n.x);
        y *= static_cast<Type>(n.y);
        z *= static_cast<Type>(n.z);
        return *this;
    }

    inline TsArrayType3<Type>& operator*= ( Type n )
    {
        x *= static_cast<Type>(n);
        y *= static_cast<Type>(n);
        z *= static_cast<Type>(n);
        return *this;
    }
    template<typename T>
    inline TsArrayType3<Type>& operator/= ( const T& n )
    {
        x /= static_cast<Type>(n.x);
        y /= static_cast<Type>(n.y);
        z /= static_cast<Type>(n.z);
        return *this;
    }

    inline TsArrayType3<Type>& operator/= ( Type n )
    {
        x /= static_cast<Type>(n);
        y /= static_cast<Type>(n);
        z /= static_cast<Type>(n);
        return *this;
    }

    inline Type	 Sum()const
    {
        return x + y + z;
    }

    inline Type Length()const
    {
        return static_cast<Type>(pow((x * x) + (y * y) + (z * z), 0.5));
    }

    inline void Normalize()
    {
        Type length = Length();
        x /= length;
        y /= length;
        z /= length;
    }

    static TsArrayType3<Type> Cross( const TsArrayType3<Type>& a , const TsArrayType3<Type>& b )
    {
        return TsArrayType3<Type>( ( a.y * b.z ) - ( a.z * b.y ) , ( a.z * b.x ) - ( a.x * b.z ) , ( a.x * b.y ) - ( a.y * b.x ) );
    }

    static Type Dot( const TsArrayType3<Type>& a , const TsArrayType3<Type>& b )
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    TsBool operator ==( const TsArrayType3<Type>& b )const
    {
        return x == b.x && y == b.y && z == b.z ;
    }

    TsBool operator !=( const TsArrayType3<Type>& b )const
    {
        return !(*this == b);
    }
};

//!*******************************************************
//!	[クラス] 4つのデータを持つクラス
//!
template<typename Type>
class TsArrayType4
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

    explicit TsArrayType4( Type _x = 0 ,
            Type _y = 0 ,
            Type _z = 0 ,
            Type _w = 0 )
    {
        x = static_cast<Type>(_x);
        y = static_cast<Type>(_y);
        z = static_cast<Type>(_z);
        w = static_cast<Type>(_w);
    }

    TsArrayType4( TsArrayType3<Type> n , Type _w )
    {
        x = n.x;
        y = n.y;
        z = n.z;
        w = _w;
    }

    TsArrayType4( TsArrayType2<Type> n , Type _z , Type _w )
    {
        x = n.x;
        y = n.y;
        z = _z;
        w = _w;
    }

    template<typename T>
    TsArrayType4( T* _array )
    {
        x = static_cast<Type>(_array[0]);
        y = static_cast<Type>(_array[1]);
        z = static_cast<Type>(_array[2]);
        w = static_cast<Type>(_array[3]);
    }
    template<typename T>
    TsArrayType4( const T & n )
    {
        *this = n;
    }

    inline const Type& operator[]( int n )const
    {
        return m[n];
    }


    inline Type& operator[]( int n )
    {
        return m[n];
    }

    template<typename T>
    inline TsArrayType4<Type>& operator = ( const T& n )
    {
        x = static_cast<Type>(n.x);
        y = static_cast<Type>(n.y);
        z = static_cast<Type>(n.z);
        w = static_cast<Type>(n.w);
        return *this;
    }
    template<typename T>
    inline TsArrayType4<Type> operator+ ( const T& n )const
    {
        return TsArrayType4<Type>( x + static_cast<Type>(n.x) , y + static_cast<Type>(n.y) , z + static_cast<Type>(n.z) , w + static_cast<Type>(n.w) );
    }
    template<typename T>
    inline TsArrayType4<Type> operator- ( const T& n )const
    {
        return TsArrayType4<Type>( x - static_cast<Type>(n.x) , y - static_cast<Type>(n.y) , z - static_cast<Type>(n.z) , w + static_cast<Type>(n.w) );
    }
    template<typename T>
    inline TsArrayType4<Type> operator* ( const T& n )const
    {
        return TsArrayType4( x*static_cast<Type>(n.x) , y * static_cast<Type>(n.y) , z*static_cast<Type>(n.z) , w + static_cast<Type>(n.w) );
    }

    inline TsArrayType4<Type> operator* ( Type n )const
    {
        return TsArrayType4( x*static_cast<Type>(n) , y*static_cast<Type>(n) , z*static_cast<Type>(n) , w*static_cast<Type>(n) );
    }
    template<typename T>
    inline TsArrayType4<Type> operator/ ( const T& n )const
    {
        return TsArrayType4( x / static_cast<Type>(n.x) , y / static_cast<Type>(n.y) , z / static_cast<Type>(n.z) , w / static_cast<Type>(n.z) );
    }

    inline TsArrayType4<Type> operator/ ( Type n )const
    {
        return TsArrayType4( x / static_cast<Type>(n) , y / static_cast<Type>(n) , z / static_cast<Type>(n) , w / static_cast<Type>(n) );
    }

    template<typename T>
    inline TsArrayType4<Type>& operator+= ( const T& n )
    {
        x += static_cast<Type>(n.x);
        y += static_cast<Type>(n.y);
        z += static_cast<Type>(n.z);
        w += static_cast<Type>(n.w);
        return *this;
    }
    template<typename T>
    inline TsArrayType4<Type>& operator-= ( const T& n )
    {
        x -= static_cast<Type>(n.x);
        y -= static_cast<Type>(n.y);
        z -= static_cast<Type>(n.z);
        w -= static_cast<Type>(n.w);
        return *this;
    }
    template<typename T>
    inline TsArrayType4<Type>& operator*= ( const T& n )
    {
        x *= static_cast<Type>(n.x);
        y *= static_cast<Type>(n.y);
        z *= static_cast<Type>(n.z);
        w *= static_cast<Type>(n.w);
        return *this;
    }

    inline TsArrayType4<Type>& operator*= ( Type n )
    {
        x *= static_cast<Type>(n);
        y *= static_cast<Type>(n);
        z *= static_cast<Type>(n);
        w *= static_cast<Type>(n);
        return *this;
    }
    template<typename T>
    inline TsArrayType4<Type>& operator/= ( const T& n )
    {
        x /= static_cast<Type>(n.x);
        y /= static_cast<Type>(n.y);
        z /= static_cast<Type>(n.z);
        w /= static_cast<Type>(n.w);
        return *this;
    }

    inline TsArrayType4<Type>& operator/= ( Type n )
    {
        x /= static_cast<Type>(n);
        y /= static_cast<Type>(n);
        z /= static_cast<Type>(n);
        w /= static_cast<Type>(n);
        return *this;
    }

    inline Type	 Sum()const
    {
        return x + y + z + w;
    }
    TsBool operator == (const TsArrayType4<Type>& b )const
    {
        return x == b.x && y == b.y && z == b.z && w == b.w;
    }
};
typedef TsArrayType2<TsInt>         TsInt2;
typedef TsArrayType2<TsUint>        TsUint2;
typedef TsArrayType2<TsShort>       TsShort2;
typedef TsArrayType2<TsUshort>      TsUShort2;
typedef TsArrayType2<TsS64>         TsS64_2;
typedef TsArrayType2<TsU64>         TsU64_2;
typedef TsArrayType2<TsS8>          TsS8_2;
typedef TsArrayType2<TsS8>          TsU8_2;
typedef TsArrayType2<TsF32>         TsFloat2;
typedef TsArrayType2<TsF64>         TsDouble2;

typedef TsArrayType3<TsInt>         TsInt3;
typedef TsArrayType3<TsUint>        TsUint3;
typedef TsArrayType3<TsShort>       TsShort3;
typedef TsArrayType3<TsUshort>      TsUShort3;
typedef TsArrayType3<TsS64>         TsS64_3;
typedef TsArrayType3<TsU64>         TsU64_3;
typedef TsArrayType3<TsS8>          TsS8_3;
typedef TsArrayType3<TsS8>          TsU8_3;
typedef TsArrayType3<TsF32>         TsFloat3;
typedef TsArrayType3<TsF64>         TsDouble3;

typedef TsArrayType4<TsInt>         TsInt4;
typedef TsArrayType4<TsUint>        TsUint4;
typedef TsArrayType4<TsShort>       TsShort4;
typedef TsArrayType4<TsUshort>      TsUShort4;
typedef TsArrayType4<TsS64>         TsS64_4;
typedef TsArrayType4<TsU64>         TsU64_4;
typedef TsArrayType4<TsS8>          TsS8_4;
typedef TsArrayType4<TsS8>          TsU8_4;
typedef TsArrayType4<TsF32>         TsFloat4;
typedef TsArrayType4<TsF64>         TsDouble4;