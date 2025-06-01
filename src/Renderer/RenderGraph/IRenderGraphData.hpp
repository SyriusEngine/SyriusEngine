#pragma once

#include "../../Communication/DispatcherManager.hpp"

namespace Syrius::Renderer {

    class RenderGraphContainer;

    class IRenderGraphData {
    public:
        IRenderGraphData(const ResourceView<Context>& ctx, RenderGraphContainer* container):
        m_Ctx(ctx),
        m_Container(container){}

        virtual ~IRenderGraphData() = default;

    protected:
        const ResourceView<Context>& m_Ctx;
        RenderGraphContainer* m_Container;
    };
}