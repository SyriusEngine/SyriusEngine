#include "LightPass.hpp"
#include "GBufferPass.hpp"
#include "GeometryPass.hpp"
#include "LightDataPass.hpp"

namespace Syrius{

    LightPass::LightPass(ResourceView<Context> &context, Resource<ShaderLibrary>& shaderLibrary):
    RenderPass(context, shaderLibrary, "LightPass", PassIDGenerator<LightPass>::getID()){
        addDependency<GeometryPass>();
        addDependency<GBufferPass>();
        addDependency<LightDataPass>();

        setupScreenQuad();
    }

    LightPass::~LightPass() {

    }

    void LightPass::execute(PassMap &passMap) {
        auto gbp = passMap.getPass<GBufferPass>();

        m_Context->beginRenderPass();

        m_VertexArray->bind();
        m_Shader->bind();
        gbp->bindShaderResources();
        m_Context->draw(m_VertexArray);

        m_Context->endRenderPass();
    }

    void LightPass::setupScreenQuad() {
        m_VertexLayout->addAttribute("Position", SR_FLOAT32_2);
        m_VertexLayout->addAttribute("TexCoords", SR_FLOAT32_2);

        loadShader("LightPass");

        VertexBufferDesc vboDesc;
        vboDesc.count = 4;
        vboDesc.data = s_ScreenVertices;
        vboDesc.usage = SR_BUFFER_USAGE_STATIC;
        vboDesc.layout = m_VertexLayout;
        m_VertexBuffer = m_Context->createVertexBuffer(vboDesc);

        IndexBufferDesc iboDesc;
        iboDesc.count = 6;
        iboDesc.data = s_ScreenIndices;
        iboDesc.usage = SR_BUFFER_USAGE_STATIC;
        iboDesc.dataType = SR_UINT32;
        m_IndexBuffer = m_Context->createIndexBuffer(iboDesc);

        VertexArrayDesc vaDesc;
        vaDesc.vertexBuffer = m_VertexBuffer;
        vaDesc.indexBuffer = m_IndexBuffer;
        vaDesc.vertexShader = m_VertexShader;
        vaDesc.drawMode = SR_DRAW_TRIANGLES;
        m_VertexArray = m_Context->createVertexArray(vaDesc);
    }

}
