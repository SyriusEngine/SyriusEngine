#include "LightStore.hpp"

namespace Syrius::Renderer {

    LightStore::LightStore(const ResourceView<Context> &ctx) {
        ConstantBufferDesc cbDesc;
        cbDesc.name = "LightData";
        cbDesc.size = sizeof(LightData);
        cbDesc.data = &m_LightData;
        cbDesc.shaderStage = SR_SHADER_FRAGMENT;
        cbDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
        m_LightDataBuffer = ctx->createConstantBuffer(cbDesc);
    }

    void LightStore::createLight(LightID lightID, const Light &light) {
        if (m_KeyLightMap.find(lightID) != m_KeyLightMap.end()) {
            SR_LOG_WARNING("LightStore", "Light {} already created!", lightID);
            return;
        }

        const auto currentIndex = m_LightData.m_LightCount.x;
        m_LightData.positions[currentIndex] = glm::vec4(light.position, 1.0f);
        m_LightData.colors[currentIndex] = glm::vec4(light.color, 1.0f);

        m_KeyLightMap.insert({lightID, currentIndex});
        m_LightData.m_LightCount.x++;

        setData();

        SR_POSTCONDITION(m_KeyLightMap.find(lightID) != m_KeyLightMap.end(), "Failed to create light {}", lightID);
    }

    void LightStore::setLight(LightID lightID, const Light &light) {
        if (m_KeyLightMap.find(lightID) == m_KeyLightMap.end()) {
            SR_LOG_WARNING("LightStore", "Light {} does not exist!", lightID);
            return;
        }
        const LightID currentIndex = m_KeyLightMap[lightID];
        m_LightData.positions[currentIndex] = glm::vec4(light.position, 1.0f);
        m_LightData.colors[currentIndex] = glm::vec4(light.color, 1.0f);

        setData();
    }

    void LightStore::destroyLight(LightID lightID) {
        if (m_KeyLightMap.find(lightID) == m_KeyLightMap.end()) {
            SR_LOG_WARNING("LightStore", "Light {} does not exist!", lightID);
            return;
        }

        auto lastIndex = m_LightData.m_LightCount.x - 1;
        for (const auto& pair: m_KeyLightMap){
            if (lastIndex == pair.second){
                //switch current light with last light
                m_LightData.positions[m_KeyLightMap[lightID]] = m_LightData.positions[lastIndex];
                m_LightData.colors[m_KeyLightMap[lightID]] = m_LightData.colors[lastIndex];
                //clear last light
                m_LightData.positions[lastIndex] = glm::vec4(0.0f);
                m_LightData.colors[lastIndex] = glm::vec4(0.0f);
            }
        }

        m_LightData.m_LightCount.x--;
        m_KeyLightMap.erase(lightID);

        setData();

        SR_POSTCONDITION(m_KeyLightMap.find(lightID) == m_KeyLightMap.end(), "Failed to remove light {}", lightID);
    }

    void LightStore::bind(const u32 slot) const {
        m_LightDataBuffer->bindShaderResource(slot);
    }

    void LightStore::setData() const {
        m_LightDataBuffer->setData(&m_LightData, sizeof(LightData));
    }
}
