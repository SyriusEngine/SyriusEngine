#pragma once

#include <SyriusEngine/Utils/EngineInclude.hpp>
#include "AccessFunctions.hpp"
#include "../Threading/WorkerPool.hpp"

namespace Syrius {

    class IDispatcher {
    public:
        virtual ~IDispatcher() = default;
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

    private:

        template<typename FUNCTION_MAP, typename... Args>
        void dispatchHelper(const FUNCTION_MAP& workerMap, Args&&... args) {
            // Make a copy of the arguments to avoid dangling references
            auto copiedArgs = std::make_tuple(std::forward<Args>(args)...);

            for (const auto& [workerType, handles]: workerMap) {
                // The vector of handles will never be that big that copying the vector will be a problem.
                // But this will ensure that no weird concurrency issues occur when EX registering a Create while
                // a Worker is running this function
                m_WorkerPool->addTask(workerType, [handles, copiedArgs] {
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