#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>

#include "../RenderGraphDefs.hpp"
#include "../Stores/ShaderStore.hpp"

namespace Syrius::Renderer {

    class ScreenQuad {
    public:
        ScreenQuad(const ShaderProgram& shaderProgram, const ResourceView<Context>& ctx);

        ~ScreenQuad() = default;

        void draw(const ResourceView<Context>& ctx) const;

    private:
        ResourceView<VertexBuffer> m_VertexBuffer;
        ResourceView<IndexBuffer> m_IndexBuffer;
        ResourceView<VertexArray> m_VertexArray;
        ResourceView<VertexLayout> m_VertexLayout;
    };
}