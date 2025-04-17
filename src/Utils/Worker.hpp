#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <list>
#include <thread>

#include <SyriusEngine/Utils/EngineInclude.hpp>

namespace Syrius {

    class Worker {
    public:
        explicit Worker(const std::string& name);

        ~Worker();

        /**
         * @brief Waits for the worker to finish processing all tasks and then
         *        stops the worker.
         */
        void stop();

        /**
          * @brief Clears the job queue and stops the worker.
          */
        void forceStop();

        template<typename... Args>
        void add(Args&&... args) {
            if (!m_IsRunning) {
                SR_LOG_WARNING(m_Name, "Worker is not running. Task will not be added.");
            }
            {
                std::lock_guard lock(m_Mutex);
                m_Queue.emplace_back(std::bind<Args>(args)...);
            }
            m_Condition.notify_all();
        }

        template<typename... Args>
        void addSync(Args&&... args) {
            bool finished = false;

            {
                std::lock_guard<std::mutex> lk(m_Mutex);
                m_Queue.push_back([func = std::bind(std::forward<Args>(args)...), &finished](){
                    func();
                    finished = true;
                });
            }

            m_Condition.notify_all();

            while (!finished){
                std::this_thread::sleep_for(1.0ms);
            }
        }

    private:

        void run();

    private:
        std::thread m_Thread;
        std::list<std::function<void()>> m_Queue;
        std::mutex m_Mutex;
        std::condition_variable m_Condition;

        bool m_IsRunning = false;
        std::string m_Name;
    };

}