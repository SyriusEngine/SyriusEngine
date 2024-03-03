#pragma once

#include "Pass.hpp"
#include "ShaderLibrary.hpp"

namespace Syrius{

    class RenderPass: public Pass{
    public:
        RenderPass(ResourceView<Context>& context, Resource<ShaderLibrary>& shaderLibrary, const std::string& name, PassID id);

        ~RenderPass() override;

    protected:

        void loadShader(const std::string& name);

    protected:
        Resource<ShaderLibrary>& m_ShaderLibrary;

        ResourceView<Shader> m_Shader;
        ResourceView<ShaderModule> m_VertexShader;
        ResourceView<ShaderModule> m_FragmentShader;

        ResourceView<VertexLayout> m_VertexLayout;
    };

}