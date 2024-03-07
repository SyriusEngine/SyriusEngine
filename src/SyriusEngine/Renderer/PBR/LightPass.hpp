#pragma once

#include "../RenderPass.hpp"

namespace Syrius{

    struct CaptureIndexData{
        glm::ivec4 attachmentIndex = glm::ivec4(0);
    };

    struct CaptureProjectionData{
        glm::mat4 perspective = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
    };

    class LightPass: public RenderPass{
    public:
        LightPass(ResourceView<Context>& context, Resource<ShaderLibrary>& shaderLibrary);

        ~LightPass() override;

        void execute(PassMap& passMap) override;

    private:

        void setupScreenQuad();

    private:
        ResourceView<VertexBuffer> m_VertexBuffer;
        ResourceView<IndexBuffer> m_IndexBuffer;
        ResourceView<VertexArray> m_VertexArray;

    };

}