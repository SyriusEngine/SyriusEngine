#pragma once

#include "../RenderPass.hpp"

namespace Syrius{

    constexpr uint32 SKYBOX_MAP_SIZE = 512;

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

    private:

        ResourceView<FrameBuffer> m_CaptureFrameBuffer;
        ResourceView<Shader> m_EquirectangularConversionShader;

    };
}