#pragma once

#include <SyriusEngine/SyriusEngine.hpp>

#include "LayerStack.hpp"
#include "Communication/DispatcherManager.hpp"

namespace Syrius {

    class SyriusEngine::EngineData {
    public:
        EngineData() = default;

    public:
        LayerStack layerStack;
        DispatcherManager dispatcherManager;
    };

}

