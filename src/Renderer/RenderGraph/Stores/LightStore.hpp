#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>
#include <SyriusEngine/Utils/EngineLimits.hpp>

#include "../IRenderGraphData.hpp"

namespace Syrius::Renderer {

    struct LightData {
        glm::vec4 positions[SR_MAX_LIGHTS];
        glm::vec4 colors[SR_MAX_LIGHTS];
        glm::uvec4 m_LightCount = {0, 0, 0, 0}; // align to 16 bytes
    };

    class LightStore: public IRenderGraphData {
    public:
        LightStore(const ResourceView<Context>& ctx, RenderGraphContainer* container, const SP<DispatcherManager>& dispatcherManager);

        ~LightStore() override = default;

        void createLight(LightID lightID, const SP<Light>& light);

        void setLight(LightID lightID, const SP<Light>& light);

        void destroyLight(LightID lightID);

        void bind(u32 slot) const;

    private:

        void setData() const;

    private:
        ResourceView<ConstantBuffer> m_LightDataBuffer;

        std::unordered_map<LightID, Index> m_KeyLightMap;
        LightData m_LightData;

    };
}