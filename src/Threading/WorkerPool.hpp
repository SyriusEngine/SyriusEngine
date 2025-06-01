#pragma once

#include "ThreadingDefs.hpp"
#include "Worker.hpp"

#include <unordered_map>

namespace Syrius {

    class WorkerPool {
    public:
        explicit WorkerPool();

        ~WorkerPool();

#if defined(SR_TESTING) || defined(SR_NO_MULTITHREADING) // For testing purposes, we always disable multithreading
        template<typename... Args>
        void addTask(const SR_WORKER_TYPE worker, Args&&... args) {
            auto& task = std::get<0>(std::forward_as_tuple(args...));
            task();
        }

        template<typename... Args>
        void addTaskSync(const SR_WORKER_TYPE worker, Args&&... args) {
            auto& task = std::get<0>(std::forward_as_tuple(args...));
            task();
        }
#else

        template<typename... Args>
       void addTask(const SR_WORKER_TYPE worker, Args&&... args) {
            m_Pool.at(worker).add(std::forward<Args>(args)...);
        }

        template<typename... Args>
        void addTaskSync(const SR_WORKER_TYPE worker, Args&&... args) {
            m_Pool.at(worker).addSync(std::forward<Args>(args)...);
        }
#endif

    private:
        std::unordered_map<SR_WORKER_TYPE, Worker> m_Pool;
    };

}