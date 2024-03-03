#include "CameraDataPass.hpp"

namespace Syrius{

    CameraDataPass::CameraDataPass(ResourceView<Context> &context, CameraDataPassDesc &desc):
    Pass(context, "CameraData", PassIDGenerator<CameraDataPass>::getID()),
    m_Slot(desc.slot),
    m_Data(){
        m_Data.view = glm::mat4(1.0f);
        m_Data.position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        ConstantBufferDesc cbDesc;
        cbDesc.name = desc.bufferName;
        cbDesc.size = sizeof(CameraData);
        cbDesc.data = &m_Data;
        cbDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
        cbDesc.shaderStage = SR_SHADER_VERTEX;
        m_CameraBuffer = m_Context->createConstantBuffer(cbDesc);
    }

    CameraDataPass::~CameraDataPass() {

    }

    void CameraDataPass::execute(PassMap& passMap) {
        m_CameraBuffer->bind(m_Slot);
    }

    void CameraDataPass::setCameraData(const glm::mat4 &viewMat, const glm::vec3 &camPos) {
        m_Data.view = viewMat;
        m_Data.position = glm::vec4(camPos, 1.0f);
        m_CameraBuffer->setData(&m_Data);
    }
}
