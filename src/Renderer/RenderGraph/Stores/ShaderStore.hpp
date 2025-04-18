#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>

namespace Syrius::Renderer {

    struct ShaderProgram {
        ResourceView<ShaderModule> vertexShader;
        ResourceView<ShaderModule> fragmentShader;
        ResourceView<Shader> shader;
    };

    class ShaderStore {
    public:
        explicit ShaderStore(const fs::path& path);

        ~ShaderStore() = default;

        const ShaderProgram& getShader(const std::string& name, const ResourceView<Context>& ctx);

    private:

        void loadShader(const std::string& name, const ResourceView<Context>& ctx);

    private:
        fs::path m_BasePath;
        std::unordered_map<std::string, ShaderProgram> m_ShaderMap;

    };

}