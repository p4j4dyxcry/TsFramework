//!*******************************************************
//!	TsHelper.h
//!
//!	Ts Helper Utilitys
//!
//!	©	2016 Yuki Tsuneyama

#pragma once

#ifdef _TS_DEBUG
	#define TsDebugLog(...)\
	{\
		TSUT::TsLog(__VA_ARGS__);\
		TSUT::TsLog("\t file::%s \n\t func::%s \n\t line::%d\n",TSUT::PassToFileName(__FILE__).c_str(),__FUNCTION__, __LINE__);\
	}
#else
	#define TsDebugLog(...)
#endif
// lapping new & app log
#define TsNew(...) \
new __VA_ARGS__ ;\
{\
	TsDebugLog("TsNew\n");\
}

// lapping delete & app log
#define TsDelete(ptr)\
TsSafeDelete(ptr);\
{\
	TsDebugLog("TsDelete\n");\
}

// safe release
template<class T>
void TsSafeRelease( T* pointer )
{
	if( pointer )
	{
		pointer->Release();
		pointer = nullptr;
	}	
}

// safe delete
template<class T>
void TsSafeDelete( T* pointer )
{
	if( pointer )
	{
		delete ( pointer );
		pointer = nullptr;
	}
}