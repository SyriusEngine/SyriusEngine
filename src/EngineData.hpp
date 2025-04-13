#pragma once

#include <SyriusEngine/SyriusEngine.hpp>
#include "LayerStack.hpp"

namespace Syrius {

    class SyriusEngine::EngineData {
    public:
        EngineData() = default;

    public:
        LayerStack layerStack;
    };

}

