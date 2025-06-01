#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>

#include "../IRenderGraphData.hpp"

namespace Syrius::Renderer {

    class ScreenQuad: public IRenderGraphData {
    public:
        ScreenQuad(const ResourceView<Context>& ctx, RenderGraphContainer* container);

        ~ScreenQuad() override = default;

        void draw(const ResourceView<Context>& ctx) const;

    private:
        ResourceView<VertexBuffer> m_VertexBuffer;
        ResourceView<IndexBuffer> m_IndexBuffer;
        ResourceView<VertexArray> m_VertexArray;
        ResourceView<VertexLayout> m_VertexLayout;
    };
}