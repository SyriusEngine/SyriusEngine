#include "LFWRSamplerPass.hpp"

namespace Syrius{

    LFWRSamplerPass::LFWRSamplerPass(ResourceView<Context> &context, uint32 slot):
    Pass(context, "LFWRSampler", PassIDGenerator<LFWRSamplerPass>::getID()),
    m_Slot(slot){
        SamplerDesc desc;
        desc.minFilter = SR_TEXTURE_FILTER_LINEAR;
        desc.magFilter = SR_TEXTURE_FILTER_LINEAR;
        desc.wrapU = SR_TEXTURE_WRAP_REPEAT;
        desc.wrapV = SR_TEXTURE_WRAP_REPEAT;
        desc.wrapW = SR_TEXTURE_WRAP_REPEAT;
        m_Sampler = m_Context->createSampler(desc);

    }

    LFWRSamplerPass::~LFWRSamplerPass() {

    }

    void LFWRSamplerPass::execute(PassMap& passMap) {
        m_Sampler->bindShaderResource(m_Slot);
    }
}