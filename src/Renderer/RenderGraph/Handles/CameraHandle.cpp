#include "CameraHandle.hpp"
#include "../RenderGraphContainer.hpp"

namespace Syrius::Renderer {

    CameraHandle::CameraHandle(const ResourceView<Context>& ctx, RenderGraphContainer* container,
                               const SP<DispatcherManager>& dispatcherManager) : IRenderGraphData(ctx, container) {
        const Camera initialCamera;
        ConstantBufferDesc cDesc;
        cDesc.name = "CameraData";
        cDesc.size = sizeof(Camera);
        cDesc.data = &initialCamera;
        cDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
        cDesc.shaderStage = SR_SHADER_VERTEX;
        m_CameraBuffer = ctx->createConstantBuffer(cDesc);

        // Dispatching Logic
        const auto cameraDispatcher = dispatcherManager->getDispatcher<CameraID, Camera>();
        cameraDispatcher->registerUpdate([this](const CameraID cameraID, const SP<Camera>& camera) {
            setCamera(cameraID, camera);
        }, SR_WORKER_RENDERER);
    }

    void CameraHandle::setCamera(CameraID cameraID, const SP<Camera>& camera) const {
        Camera cameraData = *camera;
        m_CameraBuffer->setData(&cameraData, sizeof(Camera));
    }

    void CameraHandle::bind(const u32 slot) const { m_CameraBuffer->bindShaderResource(slot); }


} // namespace Syrius::Renderer
