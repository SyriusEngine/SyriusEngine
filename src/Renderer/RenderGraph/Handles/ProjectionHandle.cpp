#include "ProjectionHandle.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../RenderGraphContainer.hpp"

namespace Syrius::Renderer {

    ProjectionHandle::ProjectionHandle(const ResourceView<Context>& ctx, RenderGraphContainer* container, const SP<DispatcherManager>& dispatcherManager):
    IRenderGraphData(ctx, container) {
        // Create buffer
        const ProjectionData data;
        ConstantBufferDesc cDesc;
        cDesc.name = "ProjectionData";
        cDesc.size = sizeof(ProjectionData);
        cDesc.data = &data;
        cDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
        cDesc.shaderStage = SR_SHADER_VERTEX;
        m_ProjectionBuffer = ctx->createConstantBuffer(cDesc);

        const SP<Projection> p = createSP<Projection>();
        p->windowWidth = ctx->getWidth();
        p->windowHeight = ctx->getHeight();
        updateProjection(SR_DEFAULT_PROJECTION, p);

        // Dispatching logic
        const auto projectionDispatcher = dispatcherManager->getDispatcher<ProjectionID, Projection>();
        projectionDispatcher->registerUpdate([this](const ProjectionID projectionID, const SP<Projection> &projection) {
            updateProjection(projectionID, projection);
        }, SR_WORKER_RENDERER);
    }

    void ProjectionHandle::updateProjection(ProjectionID projectionID, const SP<Projection>& projection) const {
        ProjectionData data;
        data.projection = glm::perspective(
                glm::radians(projection->fov),
                projection->windowWidth / projection->windowHeight,
                projection->nearPlane, projection->farPlane);
        m_ProjectionBuffer->setData(&data, sizeof(ProjectionData));
    }

    void ProjectionHandle::bind(const u32 slot) const {
        m_ProjectionBuffer->bindShaderResource(slot);
    }

}
