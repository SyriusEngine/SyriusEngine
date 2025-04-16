#pragma once

#include <SyriusEngine/Utils/EngineInclude.hpp>
#include "AccessFunctions.hpp"

namespace Syrius {

    class IDispatcher {
    public:
        virtual ~IDispatcher() = default;
    };

    template<typename T>
    class Dispatcher: public IDispatcher{
    public:
        Dispatcher() = default;

        ~Dispatcher() override = default;

        void registerCreate(CreateFunc<T> func) {
            m_Creates.push_back(func);
        }

        void registerUpdate(UpdateFunc<T> func) {
            m_Updates.push_back(func);
        }

        void registerDelete(DeleteFunc<T> func) {
            m_Deletes.push_back(func);
        }

        void dispatchCreate(const UID uid, SP<T> component) {
            for (const auto& func : m_Creates) {
                func(uid, component);
            }
        }

        void dispatchUpdate(const UID uid, SP<T> component) {
            for (const auto& func : m_Updates) {
                func(uid, component);
            }
        }

        void dispatchDelete(const UID uid) {
            for (const auto& func : m_Deletes) {
                func(uid);
            }
        }

    private:
        std::vector<CreateFunc<T>> m_Creates;
        std::vector<UpdateFunc<T>> m_Updates;
        std::vector<DeleteFunc<T>> m_Deletes;
    };

}