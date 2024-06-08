#include "SkyBoxPass.hpp"

namespace Syrius{

    SkyBoxPass::SkyBoxPass(ResourceView<Context>& context, Resource<ShaderLibrary>& shaderLibrary, const SkyBoxPassDesc& sbDesc):
    RenderPass(context, shaderLibrary, "SkyBoxPass", PassIDGenerator<SkyBoxPass>::getID()) {
        setupCaptureFrameBuffer();
        setRadianceMap(sbDesc.radianceMap);

    }

    SkyBoxPass::~SkyBoxPass() = default;

    void SkyBoxPass::execute(PassMap& passMap) {

    }

    void SkyBoxPass::setRadianceMap(const Resource<Image> &radianceMap) {

    }

    void SkyBoxPass::setupCaptureFrameBuffer() {
        auto layout = m_Context->createFrameBufferLayout();
        CubeColorAttachmentDesc ccaDesc;
        ccaDesc.width = SKYBOX_MAP_SIZE;
        ccaDesc.height = SKYBOX_MAP_SIZE;
        ccaDesc.format = SR_TEXTURE_RGBA_F16;
        ccaDesc.clearColor[0] = 0.0f;
        ccaDesc.clearColor[1] = 0.0f;
        ccaDesc.clearColor[2] = 0.0f;
        ccaDesc.clearColor[3] = 1.0f;
        layout->addCubeColorAttachmentDesc(ccaDesc);

        ViewportDesc vpDesc;
        vpDesc.width = SKYBOX_MAP_SIZE;
        vpDesc.height = SKYBOX_MAP_SIZE;
        layout->addViewportDesc(vpDesc);

        layout->configure();

        m_CaptureFrameBuffer = m_Context->createFrameBuffer(layout);
    }
}
