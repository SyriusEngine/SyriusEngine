#include "RenderPass.hpp"

namespace Syrius{

    RenderPass::RenderPass(ResourceView<Context> &context, Resource<ShaderLibrary>& shaderLibrary, const std::string& name, PassID id):
    Pass(context, name, id),
    m_ShaderLibrary(shaderLibrary){
        m_VertexLayout = m_Context->createVertexLayout();

        SR_POSTCONDITION(m_VertexLayout.get() != nullptr, "Vertex layout is null (%p)", m_VertexLayout.get());
    }

    RenderPass::~RenderPass() = default;

    void RenderPass::loadShader(const std::string& name) {
        auto& package = m_ShaderLibrary->getPackage(name);
        m_VertexShader = package.vertexShader;
        m_FragmentShader = package.fragmentShader;

        ShaderDesc shaderDesc;
        shaderDesc.vertexShader = m_VertexShader;
        shaderDesc.fragmentShader = m_FragmentShader;
        m_Shader = m_Context->createShader(shaderDesc);

        SR_POSTCONDITION(m_VertexShader.get() != nullptr, "Vertex shader module is null (%p)", m_VertexShader.get());
        SR_POSTCONDITION(m_FragmentShader.get() != nullptr, "Fragment shader module is null (%p)", m_FragmentShader.get());
        SR_POSTCONDITION(m_Shader.get() != nullptr, "Shader is null (%p)", m_Shader.get());
    }
}
