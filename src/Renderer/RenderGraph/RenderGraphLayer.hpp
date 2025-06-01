#pragma once

#include <SyriusEngine/Renderer/RenderLayer.hpp>

#include "RenderGraphContainer.hpp"
#include "RenderGraph.hpp"

namespace Syrius::Renderer {

    class RenderGraphLayer: public IRenderLayer {
    public:
        RenderGraphLayer(const fs::path& shaderPath, const SP<DispatcherManager> &dispatcherManager);
        
        ~RenderGraphLayer() override = default;

        void onRendererAttach(const ResourceView<Context>& ctx) override;

        void onRendererDetach(const ResourceView<Context>& ctx) override;

        void onRender(const ResourceView<Context>& ctx) override;

        void onResize(u32 width, u32 height, const ResourceView<Context>& ctx) override;

    private:

        void createPBRRenderGraph();

    private:
        const fs::path m_ShaderPath;

        UP<RenderGraphContainer> m_Container;
        RenderGraph m_RenderGraph;
        SP<DispatcherManager> m_DispatcherManager;
    };
}