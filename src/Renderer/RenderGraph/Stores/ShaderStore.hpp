#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>

#include "../IRenderGraphData.hpp"

namespace Syrius::Renderer {

    struct ShaderProgram {
        ResourceView<ShaderModule> vertexShader;
        ResourceView<ShaderModule> fragmentShader;
        ResourceView<Shader> shader;
    };

    class ShaderStore: public IRenderGraphData {
    public:
        ShaderStore(const ResourceView<Context>& ctx, RenderGraphContainer* container, const fs::path& path);

        ~ShaderStore() override = default;

        const ShaderProgram& getShader(const std::string& name);

    private:

        void loadShader(const std::string& name);

    private:
        fs::path m_BasePath;
        std::unordered_map<std::string, ShaderProgram> m_ShaderMap;

    };

}