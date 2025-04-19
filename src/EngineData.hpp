#pragma once

#include <SyriusEngine/SyriusEngine.hpp>

#include "LayerStack.hpp"
#include "Communication/DispatcherManager.hpp"
#include "Renderer/Renderer.hpp"

namespace Syrius {

    class SyriusEngine::EngineData {
    public:
        EngineData();

        template<typename KEY, typename DATA>
        KEY dispatchDataCreate(const DATA& data) {
            KEY key = generateID();
            const auto dispatcher = dispatcherManager->getDispatcher<KEY, DATA>();
            auto dataPtr = createSP<DATA>(data);
            dispatcher->dispatchCreate(key, dataPtr);
            return key;
        }

        template<typename KEY, typename DATA>
        void dispatchDataUpdate(KEY key, const DATA& data) {
            const auto dispatcher = dispatcherManager->getDispatcher<KEY, DATA>();
            auto dataPtr = createSP<DATA>(data);
            dispatcher->dispatchUpdate(key, dataPtr);
        }

        template<typename KEY, typename DATA>
        void dispatchDataDelete(KEY key) {
            const auto dispatcher = dispatcherManager->getDispatcher<KEY, DATA>();
            dispatcher->dispatchDelete(key);
        }

    public:
        LayerStack layerStack;
        SP<DispatcherManager> dispatcherManager;
        UP<Renderer::Renderer> renderer;
    };

}

