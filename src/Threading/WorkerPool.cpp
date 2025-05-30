#include "WorkerPool.hpp"

namespace Syrius {

    WorkerPool::WorkerPool() {
        m_Pool.emplace(std::piecewise_construct,
            std::forward_as_tuple(SR_WORKER_DEFAULT),
            std::forward_as_tuple("Default"));
        m_Pool.emplace(std::piecewise_construct,
            std::forward_as_tuple(SR_WORKER_RENDERER),
            std::forward_as_tuple("Renderer"));

    }

    WorkerPool::~WorkerPool() {
        for (auto& [type, worker]: m_Pool) {
            worker.stop();
        }
    }

}
