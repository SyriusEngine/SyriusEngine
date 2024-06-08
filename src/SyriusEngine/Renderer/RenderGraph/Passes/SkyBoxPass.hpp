#pragma once

#include "../RenderPass.hpp"

namespace Syrius{

    constexpr uint32 SKYBOX_MAP_SIZE = 512;

    struct SkyBoxProjectionData{
        glm::mat4 projection;
        glm::mat4 view;
    };

    struct SkyBoxPassDesc{
        Resource<Image> radianceMap;
    };

    class SkyBoxPass: public RenderPass{
    public:
        SkyBoxPass(ResourceView<Context>& context, Resource<ShaderLibrary>& shaderLibrary, const SkyBoxPassDesc& sbDesc);

        ~SkyBoxPass() override;

        void execute(PassMap& passMap) override;

        void setRadianceMap(const Resource<Image>& radianceMap);

    private:

        void setupCaptureFrameBuffer();

        void setupConversionObjects();

    private:
        ResourceView<VertexArray> m_SkyBoxCube;

        // for irradiance conversion
        ResourceView<FrameBuffer> m_CaptureFrameBuffer;
        ResourceView<Shader> m_IrradianceConversionShader;
        ResourceView<VertexArray> m_ConversionCube;
        ResourceView<ConstantBuffer> m_ProjectionBuffer;

    };
}