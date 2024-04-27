#include "ShaderLibrary.hpp"

namespace Syrius{

    ShaderLibrary::ShaderLibrary(ResourceView<Context> &context, const std::string &libraryPath, bool enableExperimentalSRSLShaders):
    m_Context(context),
    m_LibraryPath(libraryPath),
    m_Library(){
        SR_LOG_INFO("Shader Library (%p) uses experimental SRSL shaders: %s", this, enableExperimentalSRSLShaders ? "true" : "false");

    }

    ShaderLibrary::~ShaderLibrary() {

    }

    ShaderPackage &ShaderLibrary::getPackage(const std::string &name) {
        if (m_Library.find(name) == m_Library.end()){
            loadShader(name);
        }
        return m_Library[name];
    }

    void ShaderLibrary::loadShader(const std::string &name) {
        ShaderModuleFileDesc vsmDesc;
        vsmDesc.shaderType = SR_SHADER_VERTEX;
        vsmDesc.entryPoint = "main";

        ShaderModuleFileDesc fsmDesc;
        fsmDesc.shaderType = SR_SHADER_FRAGMENT;
        fsmDesc.entryPoint = "main";

        if (m_Context->getType() == SR_API_OPENGL){
            vsmDesc.language = SR_SHADER_LANGUAGE_GLSL;
            vsmDesc.filePath = m_LibraryPath + "/GLSL/" + name + ".vert";
            fsmDesc.language = SR_SHADER_LANGUAGE_GLSL;
            fsmDesc.filePath = m_LibraryPath + "/GLSL/" + name + ".frag";
        }
        else if (m_Context->getType() == SR_API_D3D11){
            vsmDesc.language = SR_SHADER_LANGUAGE_HLSL;
            vsmDesc.filePath = m_LibraryPath + "/HLSL/" + name + ".vs";
            fsmDesc.language = SR_SHADER_LANGUAGE_HLSL;
            fsmDesc.filePath = m_LibraryPath + "/HLSL/" + name + ".ps";
        }

        ShaderPackage package;
        package.vertexShader = m_Context->createShaderModule(vsmDesc);
        package.fragmentShader = m_Context->createShaderModule(fsmDesc);
        m_Library.insert({name, package});
    }
}
