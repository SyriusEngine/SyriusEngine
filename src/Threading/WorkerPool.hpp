#pragma once

#include "ThreadingDefs.hpp"
#include "Worker.hpp"

#include <unordered_map>

namespace Syrius {

    class WorkerPool {
    public:
        explicit WorkerPool();

        ~WorkerPool();

        template<typename... Args>
        void addTask(const SR_WORKER_TYPE worker, Args&&... args) {
            m_Pool.at(worker).add(std::forward<Args>(args)...);
        }

    private:
        std::unordered_map<SR_WORKER_TYPE, Worker> m_Pool;
    };

}