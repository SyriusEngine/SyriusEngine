#pragma once

#include "SyriusEngineInclude.hpp"
#include <mutex>

namespace Syrius{

    class SR_API Profiler{
    public:
        Profiler() = default;

        ~Profiler() = default;

        static void startTimer(const std::string& timerName);

        static void stopTimer(const std::string& timerName);

        static const std::unordered_map<std::string, uint64_t>& getDurations();

    private:
        static std::unordered_map<std::string, std::chrono::time_point<std::chrono::high_resolution_clock>> m_StartTimes;
        static std::unordered_map<std::string, uint64_t> m_Durations;
        static std::mutex m_Mutex;
    };

}

#if defined(SR_DEBUG)

#define SR_START_TIMER(name) Syrius::Profiler::startTimer(name);
#define SR_STOP_TIMER(name) Syrius::Profiler::stopTimer(name);

#else
#define SR_START_TIMER(name)
#define SR_STOP_TIMER(name)

#endif
#define SR_GET_TIMING_DATA() Syrius::Profiler::getDurations();