#pragma once

#include "../RenderGraphDefs.hpp"

namespace Syrius::Renderer {

    class GBufferHandle {
    public:
        explicit GBufferHandle(const ResourceView<Context>& ctx);

        ~GBufferHandle() = default;

        void clear() const;

        void bind(u32 startSlot) const;

        void beginRenderPass() const;

        void endRenderPass() const;

    private:
        ResourceView<FrameBuffer> m_FrameBuffer;
    };
}