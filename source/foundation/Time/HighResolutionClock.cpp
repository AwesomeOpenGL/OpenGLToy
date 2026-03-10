#include "foundation/Time/HighResolutionClock.h"

namespace Alice::Foundation::Time
{
    HighResolutionClock::HighResolutionClock()
        : m_startTime(Clock::now())
        , m_lastTime(m_startTime)
    {
    }

    double HighResolutionClock::Tick()
    {
        const Clock::time_point current = Clock::now();
        const std::chrono::duration<double> delta = current - m_lastTime;
        const std::chrono::duration<double> elapsed = current - m_startTime;

        m_lastTime = current;
        m_elapsedSeconds = elapsed.count();
        return delta.count();
    }

    double HighResolutionClock::GetElapsedSeconds() const
    {
        return m_elapsedSeconds;
    }
}
