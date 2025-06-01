#include "SamplerHandle.hpp"
#include "../RenderGraphContainer.hpp"

namespace Syrius::Renderer {

    SamplerHandle::SamplerHandle(const ResourceView<Context>& ctx, RenderGraphContainer* container):
    IRenderGraphData(ctx, container){
        SamplerDesc desc;
        desc.minFilter = SR_TEXTURE_FILTER_LINEAR;
        desc.magFilter = SR_TEXTURE_FILTER_LINEAR;
        desc.wrapU = SR_TEXTURE_WRAP_REPEAT;
        desc.wrapV = SR_TEXTURE_WRAP_REPEAT;
        desc.wrapW = SR_TEXTURE_WRAP_REPEAT;
        m_Sampler = ctx->createSampler(desc);
    }

    void SamplerHandle::bind(const u32 slot) const {
        m_Sampler->bindShaderResource(slot);
    }
}
