#include "GBufferHandle.hpp"
#include "../RenderGraphContainer.hpp"

namespace Syrius::Renderer {

    GBufferHandle::GBufferHandle(const ResourceView<Context>& ctx, RenderGraphContainer* container):
    IRenderGraphData(ctx, container){
        auto fbDesc = ctx->createFrameBufferLayout();

        /*
         * TODO: Positions stored in F16 format may cause weird artifacts with positions far from the origin
         *      Investigate if this is a problem
         *
         *      If the position is at 1000.0, the error is ~0.5, Our PBR calculations might not be accurate
         *      at 10.000 the error is ~4.0, this would cause weird problems
         */
        ColorAttachmentDesc f16Attachment;
        f16Attachment.width = ctx->getWidth();
        f16Attachment.height = ctx->getHeight();
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
        dsaDesc.width = ctx->getWidth();
        dsaDesc.height = ctx->getHeight();
        dsaDesc.format = SR_TEXTURE_DEPTH_24_STENCIL_8;
        dsaDesc.depthFunc = SR_COMPARISON_FUNC_LESS;
        dsaDesc.enableDepthTest = true;

        fbDesc->addDepthStencilAttachmentDesc(dsaDesc);

        ViewportDesc vpDesc;
        vpDesc.width = ctx->getWidth();
        vpDesc.height = ctx->getHeight();
        vpDesc.minDepth = 0.0f;
        vpDesc.maxDepth = 1.0f;
        vpDesc.xPos = 0;
        vpDesc.yPos = 0;

        fbDesc->addViewportDesc(vpDesc);

        m_FrameBuffer = ctx->createFrameBuffer(fbDesc);
    }

    void GBufferHandle::clear() const {
        m_FrameBuffer->clear();
    }

    void GBufferHandle::bind(const u32 startSlot) const {
        m_FrameBuffer->getColorAttachment(0)->bindShaderResource(startSlot);
        m_FrameBuffer->getColorAttachment(1)->bindShaderResource(startSlot + 1);
        m_FrameBuffer->getColorAttachment(2)->bindShaderResource(startSlot + 2);
        m_FrameBuffer->getColorAttachment(3)->bindShaderResource(startSlot + 3);
    }

    void GBufferHandle::beginRenderPass() const {
        m_FrameBuffer->bind();
    }

    void GBufferHandle::endRenderPass() const {
        m_FrameBuffer->unbind();
    }
}
