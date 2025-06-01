#pragma once

#include "../IRenderGraphData.hpp"

namespace Syrius::Renderer {

    class SamplerHandle: public IRenderGraphData {
    public:
        SamplerHandle(const ResourceView<Context>& ctx, RenderGraphContainer* container);

        ~SamplerHandle() override = default;

        void bind(u32 slot) const;

    private:
        ResourceView<Sampler> m_Sampler;
    };

}