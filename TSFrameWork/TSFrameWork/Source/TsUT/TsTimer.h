#pragma once

#include <chrono>

class TsTimer
{
public:
    template <typename T>
    TsF64 Elpased()const;

    TsF64 ElpasedSeccond()const;
    TsF64 ElpasedmSecond()const;
    TsF64 ElpasednSecond()const;
    TsF64 ElpasedmicroSecond()const;

    template <class T = std::chrono::milliseconds>
    TsF64 GetAvgRecodeIntarval()const;

    void Start();
    TsF64 Recode();
    void CrearRecode();
    void SetMaxRecode(TsInt m);
protected:
    std::chrono::system_clock::time_point m_startTime;
    TsInt m_maxRecode = 15;
    TsList<std::chrono::system_clock::time_point> m_recode;  
};

template < class T>
TsF64 TsTimer::GetAvgRecodeIntarval()const
{
    TsInt count = 0;
    TsS64 interval = 0;
    for (auto it = m_recode.begin(); it != m_recode.end(); ++it)
    {
        if (std::next(it) != m_recode.end())
        {
            auto s = (*it);
            auto e = (*std::next(it));
            interval += std::chrono::duration_cast<T>(e - s).count();
            count++;
        }
        else
        {
            break;
        }
    }
    if (count == 0)
        return 0;
    return (TsF64)( (TsF64)interval / (TsF64)count);
}