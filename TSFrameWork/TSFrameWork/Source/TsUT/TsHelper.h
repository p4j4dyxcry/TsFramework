//!*******************************************************
//! TsHelper.h
//!
//! Ts Helper Utilitys
//!
//! © 2016 Yuki Tsuneyama

#pragma once

#ifdef _TS_DEBUG
    #define TsDebugLog(...)\
    {\
        TSUT::TsLog(__VA_ARGS__);\
        TSUT::TsLog("\t file::%s \n\t func::%s \n\t line::%d\n",TSUT::PassToFileName(__FILE__).c_str(),__FUNCTION__, __LINE__);\
    };
#else
    #define TsDebugLog(...)
#endif

#define TsDebugLogError(...)\
    {\
        TSUT::TsConsoleColor(TSUT::TS_CONSOLE_COLOR::RED);\
        TSUT::TsLog(__VA_ARGS__);\
        TSUT::TsLog("\t file::%s \n\t func::%s \n\t line::%d\n",TSUT::PassToFileName(__FILE__).c_str(),__FUNCTION__, __LINE__);\
        TSUT::TsConsoleColor(TSUT::TS_CONSOLE_COLOR::WHITE);\
    };

// lapping new & app log
#define TsNew(...) \
new __VA_ARGS__ ;\
{\
    /*TsDebugLog("TsNew\n");*/\
}

// lapping delete & app log
#define TsDelete(ptr)\
delete (ptr);\
{\
    /*TsDebugLog("TsDelete\n");*/\
}

// safe release
#define TsSafeRelease( pointer )\
{\
    if( pointer )\
    {\
        pointer->Release();\
        pointer = nullptr;\
    }\
}

// safe delete
#define TsSafeDelete( pointer ) \
{\
    if( pointer )\
    {\
        TsDelete( pointer );\
        pointer = nullptr;\
    }\
}

//! 指定されたクラスか調べる
template<class Info , class T>
TsBool  AsType( const T& _data )
{
    printf( "%s\n%s" , typeid( Info ).name() , typeid( _data ).name() );
    return typeid( Info ).name() == typeid( _data ).name();
}

//! 指定されたクラスか調べる
template<class Info , class T>
TsBool  AsTypePtr( const T* _data )
{
    return typeid( Info ) == typeid( *_data );
}