#include "ShaderStore.hpp"
#include "../RenderGraphContainer.hpp"

namespace Syrius::Renderer {

    ShaderStore::ShaderStore(const ResourceView<Context>& ctx, RenderGraphContainer* container, const fs::path &path):
    IRenderGraphData(ctx, container),
    m_BasePath(path) {
        SR_LOG_INFO("ShaderStore", "ShaderStore will use: {} as a base dir to discover shaders", m_BasePath.string());

        SR_LOG_INFO("ShaderStore", "ShaderStore Created!");
    }

    const ShaderProgram &ShaderStore::getShader(const std::string &name) {
        if (m_ShaderMap.find(name) == m_ShaderMap.end()) {
            loadShader(name);
        }
        return m_ShaderMap.at(name);
    }

    void ShaderStore::loadShader(const std::string &name) {
        SR_PRECONDITION(m_ShaderMap.find(name) == m_ShaderMap.end(),
            "Shader {} already exists in the shader store", name);

        ShaderModuleFileDesc vsmDesc;
        vsmDesc.shaderType = SR_SHADER_VERTEX;
        vsmDesc.entryPoint = "main";

        ShaderModuleFileDesc fsmDesc;
        fsmDesc.shaderType = SR_SHADER_FRAGMENT;
        fsmDesc.entryPoint = "main";

        switch (m_Ctx->getType()) {
            case SR_API_OPENGL: {
                const std::string vertexShaderName = name + ".vert";
                const std::string fragmentShaderName = name + ".frag";
                vsmDesc.language = SR_SHADER_LANGUAGE_GLSL;
                vsmDesc.filePath = m_BasePath / "GLSL" / vertexShaderName;
                fsmDesc.language = SR_SHADER_LANGUAGE_GLSL;
                fsmDesc.filePath = m_BasePath / "GLSL" / fragmentShaderName;
                break;
            }
            case SR_API_D3D11: {
                const std::string vertexShaderName = name + ".vs";
                const std::string fragmentShaderName = name + ".ps";
                vsmDesc.language = SR_SHADER_LANGUAGE_HLSL;
                vsmDesc.filePath = m_BasePath / "HLSL" / vertexShaderName;
                fsmDesc.language = SR_SHADER_LANGUAGE_HLSL;
                fsmDesc.filePath = m_BasePath / "HLSL" / fragmentShaderName;
                break;
            }
            default: {
                SR_LOG_WARNING("ShaderStore", "ShaderStore does not support API {}", m_Ctx->getType());
                return;
            }
        }

        if (!fs::exists(vsmDesc.filePath)) {
            SR_LOG_WARNING("ShaderStore", "Vertex shader file {} does not exist", vsmDesc.filePath.string());
            return;
        }
        if (!fs::exists(fsmDesc.filePath)) {
            SR_LOG_WARNING("ShaderStore", "Fragment shader file {} does not exist", fsmDesc.filePath.string());
            return;
        }

        ShaderProgram program;
        program.vertexShader = m_Ctx->createShaderModule(vsmDesc);
        program.fragmentShader = m_Ctx->createShaderModule(fsmDesc);

        ShaderDesc sDesc;
        sDesc.vertexShader = program.vertexShader;
        sDesc.fragmentShader = program.fragmentShader;
        program.shader = m_Ctx->createShader(sDesc);

        m_ShaderMap.insert({name, program});
        SR_LOG_INFO("ShaderStore", "Loaded shader {} with vertex shader {} and fragment shader {}",
            name, vsmDesc.filePath.string(), fsmDesc.filePath.string());

        SR_PRECONDITION(m_ShaderMap.find(name) != m_ShaderMap.end(),
            "Shader {} not added in the shader store", name);
    }





}
