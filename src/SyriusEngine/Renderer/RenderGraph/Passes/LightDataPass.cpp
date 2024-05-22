#include "LightDataPass.hpp"

namespace Syrius{

    LightDataPass::LightDataPass(ResourceView<Context> &context, const LightDataPassDesc& desc):
    Pass(context, "LightData", PassIDGenerator<LightDataPass>::getID()),
    m_LightData(),
    m_Slot(desc.slot){
        for (Index i = 0; i < SR_MAX_LIGHTS; ++i){
            m_LightData.m_Position[i] = glm::vec4(0.0f);
            m_LightData.m_Color[i] = glm::vec4(0.0f);
        }
        m_LightData.m_LightCount = glm::uvec4(0);

        ConstantBufferDesc cbDesc;
        cbDesc.name = "LightData";
        cbDesc.size = sizeof(LightData);
        cbDesc.data = &m_LightData;
        cbDesc.shaderStage = SR_SHADER_FRAGMENT;
        cbDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
        m_LightDataBuffer = context->createConstantBuffer(cbDesc);
    }

    LightDataPass::~LightDataPass() {

    }

    void LightDataPass::execute(PassMap &passMap) {
        m_LightDataBuffer->bind(m_Slot);
    }

    LightID LightDataPass::createLight(const Light &light) {
        LightID lid = generateID();

        auto currentIndex = m_LightData.m_LightCount.x;
        m_LightData.m_Position[currentIndex] = glm::vec4(light.position, 1.0f);
        m_LightData.m_Color[currentIndex] = glm::vec4(light.color, 1.0f);

        m_KeyLightMap.insert({lid, currentIndex});
        m_LightData.m_LightCount.x++;

        m_LightDataBuffer->setData(&m_LightData, sizeof(LightData));

        SR_POSTCONDITION(lid != 0, "LightID cannot be %d", lid);
        return lid;
    }

    void LightDataPass::removeLight(LightID index) {
        SR_PRECONDITION(m_KeyLightMap.find(index) != m_KeyLightMap.end(), "LightID: %d does not exist", index);

        auto lastIndex = m_LightData.m_LightCount.x - 1;
        for (const auto& pair: m_KeyLightMap){
            if (lastIndex == pair.second){
                //switch current light with last light
                m_LightData.m_Position[m_KeyLightMap[index]] = m_LightData.m_Position[lastIndex];
                m_LightData.m_Color[m_KeyLightMap[index]] = m_LightData.m_Color[lastIndex];
                //clear last light
                m_LightData.m_Position[lastIndex] = glm::vec4(0.0f);
                m_LightData.m_Color[lastIndex] = glm::vec4(0.0f);
            }
        }

        m_LightData.m_LightCount.x--;
        m_LightDataBuffer->setData(&m_LightData, sizeof(LightData));

        m_KeyLightMap.erase(index);
    }

    void LightDataPass::updateLight(LightID index, const Light &light) {
        SR_PRECONDITION(m_KeyLightMap.find(index) != m_KeyLightMap.end(), "LightID: %d does not exist", index);

        auto currentIndex = m_KeyLightMap[index];
        m_LightData.m_Position[currentIndex] = glm::vec4(light.position, 1.0f);
        m_LightData.m_Color[currentIndex] = glm::vec4(light.color, 1.0f);

        m_LightDataBuffer->setData(&m_LightData, sizeof(LightData));

    }

}
