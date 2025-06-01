#include "ScreenQuad.hpp"

#include "../RenderGraphContainer.hpp"
#include "../RenderGraphDefs.hpp"
#include "../Stores/ShaderStore.hpp"

namespace Syrius::Renderer {

    static float s_ScreenVertices[] = {
        // positions        // texture Coords
        -1.0f,  -1.0f,      0.0f, 0.0f,
        -1.0f,   1.0f,      0.0f, 1.0f,
        1.0f,  -1.0f,       1.0f, 0.0f,
        1.0f,   1.0f,       1.0f, 1.0f,
    };

    static u32 s_ScreenIndices[] = {
        0, 1, 2,
        1, 2, 3
    };

    ScreenQuad::ScreenQuad(const ResourceView<Context>& ctx, RenderGraphContainer* container):
    IRenderGraphData(ctx, container){
        m_VertexLayout = ctx->createVertexLayout();
        m_VertexLayout->addAttribute("Position", SR_FLOAT32_2);
        m_VertexLayout->addAttribute("TexCoords", SR_FLOAT32_2);

        VertexBufferDesc vboDesc;
        vboDesc.count = 4;
        vboDesc.data = s_ScreenVertices;
        vboDesc.usage = SR_BUFFER_USAGE_STATIC;
        vboDesc.layout = m_VertexLayout;
        m_VertexBuffer = ctx->createVertexBuffer(vboDesc);

        IndexBufferDesc iboDesc;
        iboDesc.count = 6;
        iboDesc.data = s_ScreenIndices;
        iboDesc.usage = SR_BUFFER_USAGE_STATIC;
        iboDesc.dataType = SR_UINT32;
        m_IndexBuffer = ctx->createIndexBuffer(iboDesc);

        auto shaderStore = m_Container->getData<ShaderStore>();
        const ShaderProgram& program = shaderStore->getShader(s_LIGHT_PASS_SHADER);

        VertexArrayDesc vaDesc;
        vaDesc.vertexBuffer = m_VertexBuffer;
        vaDesc.indexBuffer = m_IndexBuffer;
        vaDesc.vertexShader = program.vertexShader;
        vaDesc.drawMode = SR_DRAW_TRIANGLES;
        m_VertexArray = ctx->createVertexArray(vaDesc);
    }

    void ScreenQuad::draw(const ResourceView<Context> &ctx) const {
        ctx->draw(m_VertexArray);
    }



}
