#pragma once

#include "AccessFunctions.hpp"
#include "Dispatcher.hpp"

#include <unordered_map>
#include <typeindex>
#include <typeinfo>

template <typename T>
inline void hash_combine(std::size_t& seed, const T& value) {
    seed ^= std::hash<T>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

struct IndexHash {
    std::size_t operator()(const std::pair<std::type_index, std::type_index>& index) const {
        std::size_t seed = 0;
        hash_combine(seed, index.first);
        hash_combine(seed, index.second);
        return seed;
    }
};

namespace Syrius {

    class DispatcherManager {
    public:
        explicit DispatcherManager(const SP<WorkerPool> &workerPool): m_WorkerPool(workerPool){}

        ~DispatcherManager() = default;

        template<typename KEY, typename DATA>
        SP<Dispatcher<KEY, DATA>> getDispatcher() {
            const Index key = { std::type_index(typeid(KEY)), std::type_index(typeid(DATA)) };
            auto it = m_Dispatchers.find(key);
            if (it == m_Dispatchers.end()) {
                m_Dispatchers[key] = std::make_shared<Dispatcher<KEY, DATA>>(m_WorkerPool);
            }
            auto dispatcher = std::static_pointer_cast<Dispatcher<KEY, DATA>>(m_Dispatchers[key]);
            if (dispatcher == nullptr) {
                SR_LOG_THROW("DispatcherManager", "Failed to cast dispatcher to the requested type");
            }
            return dispatcher;
        }

    private:
        SP<WorkerPool> m_WorkerPool = nullptr;

        using Index = std::pair<std::type_index, std::type_index>;
        std::unordered_map<Index, SP<IDispatcher>, IndexHash> m_Dispatchers;
    };
}