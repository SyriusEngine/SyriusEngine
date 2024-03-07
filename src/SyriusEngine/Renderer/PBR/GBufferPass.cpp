#include "GBufferPass.hpp"

namespace Syrius{

    GBufferPass::GBufferPass(ResourceView<Context> &context) :
            Pass(context, "GBufferPass", PassIDGenerator<GBufferPass>::getID()){
        auto fbDesc = m_Context->createFrameBufferLayout();

        ColorAttachmentDesc f32Attachment;
        f32Attachment.width = m_Context->getWidth();
        f32Attachment.height = m_Context->getHeight();
        f32Attachment.format = SR_TEXTURE_RGBA_F32;
        f32Attachment.clearColor[0] = 0.0f;
        f32Attachment.clearColor[1] = 0.0f;
        f32Attachment.clearColor[2] = 0.0f;
        f32Attachment.clearColor[3] = 0.0f;

        ColorAttachmentDesc albedoAttachment = f32Attachment;
        albedoAttachment.clearColor[0] = 0.2f;
        albedoAttachment.clearColor[1] = 0.3f;
        albedoAttachment.clearColor[2] = 0.8f;
        albedoAttachment.clearColor[3] = 1.0f;

        fbDesc->addColorAttachmentDesc(f32Attachment); // positions
        fbDesc->addColorAttachmentDesc(f32Attachment); // normals
        fbDesc->addColorAttachmentDesc(albedoAttachment); // albedo
        fbDesc->addColorAttachmentDesc(f32Attachment); // metallic + roughness + ao

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

}
