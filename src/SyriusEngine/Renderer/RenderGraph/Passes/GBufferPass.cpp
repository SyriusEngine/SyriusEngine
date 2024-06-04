#include "GBufferPass.hpp"

namespace Syrius{

    GBufferPass::GBufferPass(ResourceView<Context> &context) :
            Pass(context, "GBufferPass", PassIDGenerator<GBufferPass>::getID()){
        auto fbDesc = m_Context->createFrameBufferLayout();

        ColorAttachmentDesc f16Attachment;
        f16Attachment.width = m_Context->getWidth();
        f16Attachment.height = m_Context->getHeight();
        f16Attachment.format = SR_TEXTURE_RGBA_F16;
        f16Attachment.clearColor[0] = 0.0f;
        f16Attachment.clearColor[1] = 0.0f;
        f16Attachment.clearColor[2] = 0.0f;
        f16Attachment.clearColor[3] = 0.0f;

        ColorAttachmentDesc albedoAttachment = f16Attachment;
        albedoAttachment.clearColor[0] = 0.2f;
        albedoAttachment.clearColor[1] = 0.3f;
        albedoAttachment.clearColor[2] = 0.8f;
        albedoAttachment.clearColor[3] = 1.0f;

        fbDesc->addColorAttachmentDesc(f16Attachment); // positions
        fbDesc->addColorAttachmentDesc(f16Attachment); // normals
        fbDesc->addColorAttachmentDesc(albedoAttachment); // albedo
        fbDesc->addColorAttachmentDesc(f16Attachment); // metallic + roughness + ao

        DepthStencilAttachmentDesc dsaDesc;
        dsaDesc.width = m_Context->getWidth();
        dsaDesc.height = m_Context->getHeight();
        dsaDesc.format = SR_TEXTURE_DEPTH_24_STENCIL_8;
        dsaDesc.depthFunc = SR_COMPARISON_FUNC_LESS;
        dsaDesc.enableDepthTest = true;

        fbDesc->addDepthStencilAttachmentDesc(dsaDesc);

        ViewportDesc vpDesc;
        vpDesc.width = m_Context->getWidth();
        vpDesc.height = m_Context->getHeight();
        vpDesc.minDepth = 0.0f;
        vpDesc.maxDepth = 1.0f;
        vpDesc.xPos = 0;
        vpDesc.yPos = 0;

        fbDesc->addViewportDesc(vpDesc);

        m_FrameBuffer = m_Context->createFrameBuffer(fbDesc);

        SR_POSTCONDITION(m_FrameBuffer.get() != nullptr, "FrameBuffer is nullptr");
    }

    GBufferPass::~GBufferPass() {

    }

    void GBufferPass::execute(PassMap& passMap) {
        m_FrameBuffer->clear();
    }

    void GBufferPass::beginRenderPass() {
        m_Context->beginRenderPass(m_FrameBuffer);
    }

    void GBufferPass::endRenderPass() {
        m_Context->endRenderPass(m_FrameBuffer);
    }

    void GBufferPass::bindShaderResources() {
        m_FrameBuffer->getColorAttachment(0)->bindShaderResource(0);
        m_FrameBuffer->getColorAttachment(1)->bindShaderResource(1);
        m_FrameBuffer->getColorAttachment(2)->bindShaderResource(2);
        m_FrameBuffer->getColorAttachment(3)->bindShaderResource(3);
    }

    void GBufferPass::onResize(uint32 width, uint32 height) {
        m_FrameBuffer->onResize(width, height);
    }
}
