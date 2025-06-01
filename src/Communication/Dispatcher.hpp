#pragma once

#include <SyriusEngine/Utils/EngineInclude.hpp>
#include <iostream>
#include "../Threading/WorkerPool.hpp"
#include "AccessFunctions.hpp"
#include "SyriusEngine/Renderer/RenderPrimitives.hpp"

namespace Syrius {

    class IDispatcher {
    public:
        virtual ~IDispatcher() = default;

        virtual std::string getDebugInfo() const = 0;
    };

    template<typename KEY, typename DATA>
    class Dispatcher: public IDispatcher{
    public:
        Dispatcher(const SP<WorkerPool> &workerPool): m_WorkerPool(workerPool){}

        ~Dispatcher() override = default;

        void registerCreate(CreateFunc<KEY, DATA> func, const SR_WORKER_TYPE workerType = SR_WORKER_DEFAULT) {
            m_Creates[workerType].push_back(func);
        }

        void registerUpdate(UpdateFunc<KEY, DATA> func, const SR_WORKER_TYPE workerType = SR_WORKER_DEFAULT) {
            m_Updates[workerType].push_back(func);
        }

        void registerDelete(DeleteFunc<KEY> func, const SR_WORKER_TYPE workerType = SR_WORKER_DEFAULT) {
            m_Deletes[workerType].push_back(func);
        }

        void dispatchCreate(const KEY uid, SP<DATA> component) {
            dispatchHelper(m_Creates, uid, component);
        }

        void dispatchUpdate(const KEY uid, SP<DATA> component) {
            dispatchHelper(m_Updates, uid, component);
        }

        void dispatchDelete(const UID uid) {
            dispatchHelper(m_Deletes, uid);
        }

        std::string getDebugInfo() const override{
            std::string info = "======Dispatcher<" + std::string(typeid(KEY).name()) + ", " + std::string(typeid(DATA).name()) + ">======\n";
            info += "Creates:\n";
            for (const auto& [workerType, creates]: m_Creates) {
                info += std::to_string(workerType) + ": Functions: " + std::to_string(creates.size()) + "\n";
            }
            info += "Updates:\n";
            for (const auto& [workerType, updates]: m_Updates) {
                info += std::to_string(workerType) + ": Functions: " + std::to_string(updates.size()) + "\n";
            }
            info += "Delets:\n";
            for (const auto& [workerType, deletes]: m_Deletes) {
                info += std::to_string(workerType) + ": Functions: " + std::to_string(deletes.size()) + "\n";
            }
            return info;
        }

    private:

        template<typename FUNCTION_MAP, typename... Args>
        void dispatchHelper(const FUNCTION_MAP& workerMap, Args&&... args) {
            // Make a copy of the arguments to avoid dangling references
            auto copiedArgs = std::make_tuple(std::forward<Args>(args)...);

            for (const auto& [workerType, handles]: workerMap) {
                m_WorkerPool->addTask(workerType, [&handles, copiedArgs] {
                    std::apply([&](auto&&... unpackedArgs) {
                        for (const auto& func: handles) {
                            func(unpackedArgs...);
                        }
                    }, copiedArgs);
                });
            }
        }

    private:
        SP<WorkerPool> m_WorkerPool;

        std::unordered_map<SR_WORKER_TYPE, std::vector<CreateFunc<KEY, DATA>>> m_Creates;
        std::unordered_map<SR_WORKER_TYPE, std::vector<UpdateFunc<KEY, DATA>>> m_Updates;
        std::unordered_map<SR_WORKER_TYPE, std::vector<DeleteFunc<KEY>>> m_Deletes;
    };

}