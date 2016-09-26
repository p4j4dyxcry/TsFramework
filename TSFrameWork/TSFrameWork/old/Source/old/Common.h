//!*******************************************************
//!	Common.h
//!
//!	共通定義
//!
//!	著作権	2016 Yuki Tsuneyama
//!	作成日時	16.01.15

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memory>
#include <vector>
#include <list>
#include <unordered_map>
#include <algorithm>
#include "unknwn.h"

template <typename ReleaseObject>
void SafeRelease( ReleaseObject object )
{
	if( object )
		object->Release();
	object = nullptr;
}

template <typename Pointer>
void SafeDelete( Pointer ptr )
{
	if( ptr )
		delete ptr;
	ptr = nullptr;
}

template <typename PointerArray>
void SafeDeleteArray( PointerArray ptr )
{
	if( ptr )
		delete[] ptr;
	ptr = nullptr;
}

template <typename Object>
void Swap( Object& a , Object& b )
{
	Object temp = a;
	a = b;
	b = temp;
}

template <typename Object>
bool IsTypeEqual( Object a , Object b)
{
	return typeid( a ) == typeid( b );
}
struct D3DX11_DELETER
{
	void operator()( IUnknown* ptr_ ){
		SafeRelease( ptr_ );
	}
};

typedef const char* StaticFunc();

