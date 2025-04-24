#pragma once

#include "../RenderGraphDefs.hpp"

namespace Syrius::Renderer {

    class SamplerHandle {
    public:
        explicit SamplerHandle(const ResourceView<Context>& ctx);

        ~SamplerHandle() = default;

        void bind(u32 slot) const;

    private:
        ResourceView<Sampler> m_Sampler;
    };

}