#include "RenderPass.hpp"

namespace Syrius{

    RenderPass::RenderPass(ResourceView<Context> &context, const std::string& name, PassID id):
    Pass(context, name, id){
        m_VertexLayout = m_Context->createVertexLayout();
    }

    RenderPass::~RenderPass() {

    }

    void RenderPass::loadShader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) {
        SR_PRECONDITION(!vertexShaderPath.empty(), "Vertex shader path is empty");
        SR_PRECONDITION(!fragmentShaderPath.empty(), "Fragment shader path is empty");
        SR_PRECONDITION(std::filesystem::exists(vertexShaderPath), "Vertex shader path does not exist: %s", vertexShaderPath.c_str());
        SR_PRECONDITION(std::filesystem::exists(fragmentShaderPath), "Fragment shader path does not exist: %s", fragmentShaderPath.c_str());


        ShaderModuleDesc vsmDesc;
        vsmDesc.code = vertexShaderPath;
        vsmDesc.shaderType = SR_SHADER_VERTEX;
        vsmDesc.loadType = SR_LOAD_FROM_FILE;
        vsmDesc.codeType = SR_SHADER_CODE_GLSL;
        m_VertexShader = m_Context->createShaderModule(vsmDesc);

        ShaderModuleDesc fsmDesc;
        fsmDesc.code = fragmentShaderPath;
        fsmDesc.shaderType = SR_SHADER_FRAGMENT;
        fsmDesc.loadType = SR_LOAD_FROM_FILE;
        fsmDesc.codeType = SR_SHADER_CODE_GLSL;
        m_FragmentShader = m_Context->createShaderModule(fsmDesc);

        ShaderDesc shaderDesc;
        shaderDesc.vertexShader = m_VertexShader;
        shaderDesc.fragmentShader = m_FragmentShader;
        m_Shader = m_Context->createShader(shaderDesc);

        SR_POSTCONDITION(m_VertexShader.get() != nullptr, "Vertex shader module is null");
        SR_POSTCONDITION(m_FragmentShader.get() != nullptr, "Fragment shader module is null");
        SR_POSTCONDITION(m_Shader.get() != nullptr, "Shader is null");
    }
}
