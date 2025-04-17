#pragma once

#include <SyriusEngine/Utils/EngineInclude.hpp>
#include "AccessFunctions.hpp"

namespace Syrius {

    class IDispatcher {
    public:
        virtual ~IDispatcher() = default;
    };

    template<typename KEY, typename DATA>
    class Dispatcher: public IDispatcher{
    public:
        Dispatcher() = default;

        ~Dispatcher() override = default;

        void registerCreate(CreateFunc<KEY, DATA> func) {
            m_Creates.push_back(func);
        }

        void registerUpdate(UpdateFunc<KEY, DATA> func) {
            m_Updates.push_back(func);
        }

        void registerDelete(DeleteFunc<KEY> func) {
            m_Deletes.push_back(func);
        }

        void dispatchCreate(const KEY uid, SP<DATA> component) {
            for (const auto& func : m_Creates) {
                func(uid, component);
            }
        }

        void dispatchUpdate(const KEY uid, SP<DATA> component) {
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
        std::vector<CreateFunc<KEY, DATA>> m_Creates;
        std::vector<UpdateFunc<KEY, DATA>> m_Updates;
        std::vector<DeleteFunc<KEY>> m_Deletes;
    };

}