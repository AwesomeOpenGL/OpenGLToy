#pragma once

#include <chrono>

namespace Alice::Foundation::Time
{
    class HighResolutionClock
    {
    public:
        HighResolutionClock();

        double Tick();
        double GetElapsedSeconds() const;

    private:
        using Clock = std::chrono::high_resolution_clock;

        Clock::time_point m_startTime;
        Clock::time_point m_lastTime;
        double m_elapsedSeconds = 0.0;
    };
}
