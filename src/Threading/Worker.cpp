#include "Worker.hpp"

namespace Syrius {

    Worker::Worker(const std::string &name):
    m_Thread([this] {
            m_IsRunning = true;
            run();
        }),
    m_Name(name) {

    }

    Worker::~Worker() {
        if (m_IsRunning) {
            stop();
        }
    }

    void Worker::stop() {
        {
            std::lock_guard lk(m_Mutex);
            if (!m_IsRunning){
                return;
            }
            m_IsRunning = false;
        }

        m_Condition.notify_all();
        m_Thread.join();
    }

    void Worker::forceStop() {
        {
            std::lock_guard lk(m_Mutex);
            m_Queue.clear();
            if (!m_IsRunning){
                return;
            }
            m_IsRunning = false;
        }

        m_Condition.notify_all();
        m_Thread.join();


    }

    void Worker::run() {
        auto doTasks = [this](const std::list<std::function<void()>>& queue) {
            for (auto& func: queue){
                try {
                    func();
                }
                catch (std::exception& e) {
                    SR_LOG_ERROR(m_Name, "Current task threw an exception: {}", e.what());
                }
            }
        };

        for (;;){
            std::list<std::function<void()>> queue;
            {
                std::unique_lock lk(m_Mutex);
                m_Condition.wait(lk, [&] {
                    return !m_Queue.empty() || !m_IsRunning;
                });
                if (!m_IsRunning){
                    doTasks(m_Queue);
                    m_Queue.clear();
                    return;
                }
                std::swap(m_Queue, queue);
            }
            doTasks(queue);
        }
    }
}