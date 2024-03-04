#pragma once

#include "../Pass.hpp"

namespace Syrius{

    class LFWRSamplerPass : public Pass{
    public:
        LFWRSamplerPass(ResourceView<Context>& context, uint32 slot);

        ~LFWRSamplerPass() override;

        void execute(PassMap& passMap) override;

    private:
        ResourceView<Sampler> m_Sampler;
        uint32 m_Slot;

    };

}