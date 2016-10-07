#include "TsUT.h"

namespace TSUT
{
    struct TsMemory
    {
        void*		m_pointer;
        TsString	m_function;

        TsBool operator == ( const TsMemory& ptr )
        {
            return m_pointer == ptr.m_pointer;
        }
    };
    static TsList<TSUT::TsMemory> g_AllocMemoryList;
}

class TsDumpMemoryLeak
{
    ~TsDumpMemoryLeak()
    {
        for each( auto var in TSUT::g_AllocMemoryList )
        {
            TSUT::TsLog( "Memory Leak !! \n" );
            TSUT::TsLog( var.m_function.c_str() );
        }
    }
    static TsDumpMemoryLeak m_this;
};

TsBool TSUT::AllocMemory( void * pointer ,TsString string)
{
    TsMemory m;
    m.m_pointer = pointer;
    m.m_function = string;
    g_AllocMemoryList.push_front( m );
    return TS_TRUE;
}

TsBool TSUT::DeleteMemory( void * pointer )
{
    TsMemory m;
    m.m_pointer = pointer;
    m.m_function = "";

    g_AllocMemoryList.remove( m );
    return TS_TRUE;
}