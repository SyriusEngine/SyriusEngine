#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>
#include <functional>

#include "RenderGraphContainer.hpp"
#include "RenderGraphDefs.hpp"

namespace Syrius::Renderer {

    using Executor = std::function<void(const ResourceView<Context>&, RenderGraphContainer* )>;
    using NodeID = UID;

    struct RenderGraphNode {
        std::vector<SR_RENDER_NODE> needs;
        std::vector<SR_RENDER_NODE> provides;
        Executor executor = nullptr;
        NodeID id = generateID();
    };

}