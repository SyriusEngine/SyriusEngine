#pragma once

#include "RenderGraphInclude.hpp"

namespace Syrius{

    struct ShaderPackage{
        ResourceView<ShaderModule> vertexShader;
        ResourceView<ShaderModule> fragmentShader;
    };

    class ShaderLibrary{
    public:
        ShaderLibrary() = delete;

        ShaderLibrary(ResourceView<Context>& context, const std::string& libraryPath, bool enableExperimentalSRSLShaders);

        ~ShaderLibrary();

        ShaderPackage& getPackage(const std::string& name);

    private:

        void loadShader(const std::string& name);

    private:
        const std::string m_LibraryPath;
        ResourceView<Context>& m_Context;

        std::unordered_map<std::string, ShaderPackage> m_Library;


    };

}