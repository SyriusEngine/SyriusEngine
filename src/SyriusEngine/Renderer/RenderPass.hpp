#pragma once

#include "Pass.hpp"

namespace Syrius{

    class RenderPass: public Pass{
    public:
        RenderPass(ResourceView<Context>& context, const std::string& name, PassID id);

        ~RenderPass() override;

    protected:

        void loadShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

    protected:
        ResourceView<Shader> m_Shader;
        ResourceView<ShaderModule> m_VertexShader;
        ResourceView<ShaderModule> m_FragmentShader;

        ResourceView<VertexLayout> m_VertexLayout;
    };

}