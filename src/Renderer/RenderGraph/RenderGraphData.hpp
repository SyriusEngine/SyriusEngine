#pragma once

#include "Stores/ShaderStore.hpp"
#include "Stores/GeometryStore.hpp"

namespace Syrius::Renderer {

    struct RenderGraphData {
        UP<ShaderStore> shaderStore;
        UP<GeometryStore> geometryStore;
    };

}