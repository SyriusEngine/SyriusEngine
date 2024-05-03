#include "../../../include/SyriusEngine/Core/Profiler.hpp"

namespace Syrius{

    std::unordered_map<std::string, std::chrono::time_point<std::chrono::high_resolution_clock>> Profiler::m_StartTimes;
    std::unordered_map<std::string, uint64_t> Profiler::m_Durations;
    std::mutex Profiler::m_Mutex;


    void Profiler::startTimer(const std::string &name) {
        std::lock_guard<std::mutex> lk(m_Mutex);
        m_StartTimes[name] = std::chrono::high_resolution_clock::now();
    }

    void Profiler::stopTimer(const std::string &name) {
        auto end = std::chrono::high_resolution_clock::now();
        std::lock_guard<std::mutex> lk(m_Mutex);
        m_Durations[name] = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_StartTimes[name]).count();
    }

    const std::unordered_map<std::string, uint64_t> &Profiler::getDurations() {
        std::lock_guard<std::mutex> lk(m_Mutex);
        return m_Durations;
    }

}
