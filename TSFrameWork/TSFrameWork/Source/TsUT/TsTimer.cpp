#include "TsUT.h"

using namespace std::chrono;


template < typename T>
TsF64 TsTimer::Elpased()const
{
    auto d = std::chrono::system_clock::now() - m_startTime;
    return (TsF64)duration_cast<T>(d).count();
};

TsF64 TsTimer::ElpasedSeccond()const
{
    return Elpased<seconds>();
}
TsF64 TsTimer::ElpasedmSecond()const
{
    return Elpased<milliseconds>();
}

TsF64 TsTimer::ElpasedmicroSecond()const
{
    return Elpased<microseconds>();
}

TsF64 TsTimer::ElpasednSecond()const
{
    return Elpased<nanoseconds>();
}




void TsTimer::Start()
{
    m_startTime = std::chrono::system_clock::now();
}
TsF64 TsTimer::Recode()
{
    m_recode.push_back( std::chrono::system_clock::now() );
    if (m_recode.size() >= m_maxRecode)
        m_recode.pop_front();

    return ElpasedmSecond();
}
void TsTimer::CrearRecode()
{
    m_recode.clear();
}
void TsTimer::SetMaxRecode(TsInt m)
{
    m_maxRecode = m;
}