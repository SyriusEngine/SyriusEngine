#pragma once

#include <SyriusEngine/SyriusEngine.hpp>

#include "LayerStack.hpp"
#include "Communication/DispatcherManager.hpp"
#include "Renderer/Renderer.hpp"

namespace Syrius {

    class SyriusEngine::EngineData {
    public:
        EngineData();

    public:
        LayerStack layerStack;
        SP<DispatcherManager> dispatcherManager;
        UP<Renderer::Renderer> renderer;
    };

}

