#include "ProjectionHandle.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Syrius::Renderer {

    ProjectionHandle::ProjectionHandle(const ResourceView<Context> &ctx) {
        ProjectionData data;
        ConstantBufferDesc cDesc;
        cDesc.name = "ProjectionData";
        cDesc.size = sizeof(ProjectionData);
        cDesc.data = &data;
        cDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
        cDesc.shaderStage = SR_SHADER_VERTEX;
        m_ProjectionBuffer = ctx->createConstantBuffer(cDesc);
    }

    void ProjectionHandle::updateProjection(ProjectionID projectionID, const Projection &projection) const {
        ProjectionData data;
        data.projection = glm::perspective(
                glm::radians(projection.fov),
                projection.windowWidth / projection.windowHeight,
                projection.nearPlane, projection.farPlane);
        m_ProjectionBuffer->setData(&data, sizeof(ProjectionData));
    }

    void ProjectionHandle::bind(const u32 slot) const {
        m_ProjectionBuffer->bind(slot);
    }

}
