#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>
#include <functional>

#include "RenderGraphData.hpp"

namespace Syrius::Renderer {

    using Executor = std::function<void(const ResourceView<Context>&, RenderGraphData& )>;

    struct RenderGraphNode {
        std::string name;
        std::vector<std::string> dependencies;
        Executor executor = nullptr;

    };

}