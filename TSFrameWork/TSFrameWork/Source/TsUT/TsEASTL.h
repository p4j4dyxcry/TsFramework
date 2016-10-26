#pragma once

#ifdef _DEBUG 
#pragma comment (lib,"Release\\libtcmalloc_minimal.lib")
#pragma comment (lib,"Debug\\EASTL.lib")
#else
#pragma comment (lib,"Release\\libtcmalloc_minimal.lib")
#pragma comment (lib,"Release\\EASTL.lib")
#endif

#include <EASTL\vector.h>
#include <EASTL\list.h>
#include <EASTL\map.h>
#include <EASTL\stack.h>

// EASTL expects us to define these, see allocator.h line 194
static void* operator new[]( size_t size , const char* pName , int flags ,
                      unsigned debugFlags , const char* file , int line )
{
    return malloc( size );
}
static void* operator new[]( size_t size , size_t alignment , size_t alignmentOffset ,
const char* pName , int flags , unsigned debugFlags , const char* file , int line )
{
    // this allocator doesn't support alignment
    EASTL_ASSERT( alignment <= 8 );
    return malloc( size );
}