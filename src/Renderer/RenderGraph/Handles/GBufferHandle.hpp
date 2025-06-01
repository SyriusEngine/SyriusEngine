#pragma once

#include "../RenderGraphDefs.hpp"
#include "../IRenderGraphData.hpp"

namespace Syrius::Renderer {

    class GBufferHandle: public IRenderGraphData {
    public:
        GBufferHandle(const ResourceView<Context>& ctx, RenderGraphContainer* container);

        ~GBufferHandle() override = default;

        void clear() const;

        void bind(u32 startSlot) const;

        void beginRenderPass() const;

        void endRenderPass() const;

    private:
        ResourceView<FrameBuffer> m_FrameBuffer;
    };
}