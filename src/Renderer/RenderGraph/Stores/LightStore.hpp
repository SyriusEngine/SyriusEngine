#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>
#include <SyriusEngine/Utils/EngineLimits.hpp>

#include "../RenderGraphDefs.hpp"

namespace Syrius::Renderer {

    struct LightData {
        glm::vec4 positions[SR_MAX_LIGHTS];
        glm::vec4 colors[SR_MAX_LIGHTS];
        glm::uvec4 m_LightCount = {0, 0, 0, 0}; // align to 16 bytes
    };

    class LightStore {
    public:
        explicit LightStore(const ResourceView<Context>& ctx);

        ~LightStore() = default;

        void createLight(LightID lightID, const Light& light);

        void setLight(LightID lightID, const Light& light);

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