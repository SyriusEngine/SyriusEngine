#include "../../../include/SyriusEngine/Utils/Worker.hpp"

namespace Syrius{

    Worker::Worker():
    m_IsRunning(true),
    m_Thread([this]{
        threadFunc();
    }){

    }

    Worker::~Worker() {
        if (m_IsRunning){
            stop();
        }
    }

    void Worker::stop() {
        {
            std::lock_guard<std::mutex> lk(m_Mutex);
            if (!m_IsRunning){
                return;
            }
            m_IsRunning = false;
        }

        m_Condition.notify_all();
        m_Thread.join();
    }

    void Worker::threadFunc() {
        for (;;){
            decltype(m_Queue) queue;
            {
                std::unique_lock<std::mutex> lk(m_Mutex);
                m_Condition.wait(lk, [&] {
                    return !m_Queue.empty() + !m_IsRunning;
                });
                if (!m_IsRunning){
                    for (auto& func: m_Queue){
                        func();
                    }
                    m_Queue.clear();
                    return;
                }
                std::swap(m_Queue, queue);
            }
            for (auto& func: queue){
                func();
            }
        }
    }
}
