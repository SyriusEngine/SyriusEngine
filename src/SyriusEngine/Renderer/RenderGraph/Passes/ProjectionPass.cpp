#include "ProjectionPass.hpp"

namespace Syrius{

    ProjectionPass::ProjectionPass(ResourceView<Context> &context, ProjectionPassDesc &desc):
    Pass(context, "ProjectionData", PassIDGenerator<ProjectionPass>::getID()),
    m_Slot(desc.slot),
    m_Data(),
    m_Projection(desc.projection){
        m_Data.perspective = glm::perspective(
                glm::radians(desc.projection.fov),
                desc.projection.windowWidth / desc.projection.windowHeight,
                desc.projection.nearPlane, desc.projection.farPlane);
        m_Data.orthographic = glm::ortho(-desc.projection.windowWidth / 2.0f, desc.projection.windowWidth / 2.0f,
                                         -desc.projection.windowHeight / 2.0f, desc.projection.windowHeight / 2.0f,
                                         desc.projection.nearPlane, desc.projection.farPlane);

        ConstantBufferDesc cDesc;
        cDesc.name = desc.bufferName;
        cDesc.size = sizeof(ProjectionData);
        cDesc.data = &m_Data;
        cDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
        cDesc.shaderStage = SR_SHADER_VERTEX;
        m_ProjectionBuffer = context->createConstantBuffer(cDesc);

    }

    ProjectionPass::~ProjectionPass() = default;

    void ProjectionPass::execute(PassMap& passMap) {
        m_ProjectionBuffer->bind(m_Slot);
    }

    void ProjectionPass::onResize(uint32 width, uint32 height) {
        m_Projection.windowWidth = static_cast<float>(width);
        m_Projection.windowHeight = static_cast<float>(height);

        update();
    }

    void ProjectionPass::setFOV(float fov) {
        m_Projection.fov = fov;

        update();
    }

    void ProjectionPass::setPlane(float nearPlane, float farPlane) {
        m_Projection.nearPlane = nearPlane;
        m_Projection.farPlane = farPlane;

        update();
    }

    void ProjectionPass::update() {
        m_Data.perspective = glm::perspective(
                glm::radians(m_Projection.fov),
                m_Projection.windowWidth / m_Projection.windowHeight,
                m_Projection.nearPlane, m_Projection.farPlane);
        m_Data.orthographic = glm::ortho(0.0f, m_Projection.windowWidth,
                                         0.0f, m_Projection.windowHeight,
                                         m_Projection.nearPlane, m_Projection.farPlane);

        m_ProjectionBuffer->setData(&m_Data, sizeof(ProjectionData));
    }
}