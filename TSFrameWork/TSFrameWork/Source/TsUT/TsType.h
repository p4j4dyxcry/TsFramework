//!*******************************************************
//!	TsType.h
//!
//!	TsFramework Types
//!
//!	© 2016 Yuki Tsuneyama

#pragma once

typedef unsigned int		TsUint;
typedef int					TsInt;
typedef float				TsF32;
typedef double				TsF64;
typedef wchar_t				TsLChar;
typedef char				TsChar;
typedef char				TsS8;
typedef unsigned char		TsU8;
typedef short				TsShort;
typedef unsigned short		TsUshort;
typedef TsU8				TsByte;
typedef bool				TsBool;
typedef long long			TsS64;
typedef unsigned long long	TsU64;
typedef std::string			TsString;

#define TsList				std::list
#define TsVector			std::vector
#define TsMap				std::unordered_map
#define TsStack				std::stack

#define TS_TRUE				true
#define TS_FALSE			false

typedef TsU64 TS_HASH;