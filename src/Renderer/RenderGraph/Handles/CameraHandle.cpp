#include "CameraHandle.hpp"

namespace Syrius::Renderer {

    CameraHandle::CameraHandle(const ResourceView<Context> &ctx) {
        Camera camera;
        ConstantBufferDesc cDesc;
        cDesc.name = "CameraData";
        cDesc.size = sizeof(Camera);
        cDesc.data = &camera;
        cDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
        cDesc.shaderStage = SR_SHADER_VERTEX;
        m_CameraBuffer = ctx->createConstantBuffer(cDesc);
    }

    void CameraHandle::setCamera(CameraID cameraID, const Camera &camera) const {
        m_CameraBuffer->setData(&camera, sizeof(Camera));
    }

    void CameraHandle::bind(u32 slot) const {
        m_CameraBuffer->bindShaderResource(slot);
    }



}
