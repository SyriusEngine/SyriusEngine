#pragma once

#include "AccessFunctions.hpp"
#include "Dispatcher.hpp"

#include <unordered_map>
#include <typeindex>
#include <typeinfo>

namespace Syrius {

    class DispatcherManager {
    public:
        DispatcherManager() = default;

        ~DispatcherManager() = default;

        template<typename T>
        SP<Dispatcher<T>> getDispatcher() {
            auto type = std::type_index(typeid(T));
            auto it = m_Dispatchers.find(type);
            if (it == m_Dispatchers.end()) {
                m_Dispatchers[type] = std::make_shared<Dispatcher<T>>();
            }
            return std::static_pointer_cast<Dispatcher<T>>(m_Dispatchers[type]);
        }

    private:
        std::unordered_map<std::type_index, SP<IDispatcher>> m_Dispatchers;
    };
}