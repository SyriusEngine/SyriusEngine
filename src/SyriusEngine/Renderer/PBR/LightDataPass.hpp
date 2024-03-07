#pragma once

#include "../Pass.hpp"

namespace Syrius{

    struct LightDataPassDesc{
        uint32 slot = 3;
    };

    struct LightData{
        glm::vec4 m_Position[SR_MAX_LIGHTS];
        glm::vec4 m_Color[SR_MAX_LIGHTS];
        glm::uvec4 m_LightCount = {0, 0, 0, 0}; // align to 16 bytes boundary
    };

    class LightDataPass: public Pass{
    public:
        LightDataPass(ResourceView<Context>& context, const LightDataPassDesc& desc);

        ~LightDataPass() override;

        void execute(PassMap& passMap) override;

        LightID createLight(const Light& light);

        void removeLight(LightID index);

        void updateLight(LightID index, const Light& light);

    private:
        ResourceView<ConstantBuffer> m_LightDataBuffer;

        LightData m_LightData;
        std::unordered_map<LightID, Index> m_KeyLightMap;
        uint32 m_Slot;
    };

}