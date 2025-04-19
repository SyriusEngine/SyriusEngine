#include "RenderGraphLayer.hpp"

#include "RenderGraphDefs.hpp"

namespace Syrius::Renderer {

    RenderGraphLayer::RenderGraphLayer(const fs::path &shaderPath) {
        m_RenderGraphData.shaderStore = createUP<ShaderStore>(shaderPath);
    }

    void RenderGraphLayer::onRendererAttach(const ResourceView<Context> &ctx) {
        SR_PRECONDITION(m_RenderGraphData.shaderStore != nullptr, "Shader store is not initialized");

        m_RenderGraphData.geometryStore = createUP<GeometryStore>(m_RenderGraphData.shaderStore, ctx);

        TransformData transformData[SR_MAX_INSTANCES];
        ConstantBufferDesc cbDesc;
        cbDesc.name = "ModelData";
        cbDesc.size = sizeof(TransformData) * SR_MAX_INSTANCES;
        cbDesc.shaderStage = SR_SHADER_VERTEX;
        cbDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
        cbDesc.data = &transformData;
        m_ModelData = ctx->createConstantBuffer(cbDesc);
    }

    void RenderGraphLayer::onRendererDetach(const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::onRender(const ResourceView<Context> &ctx) {
        const ShaderProgram& shaderProgram = m_RenderGraphData.shaderStore->getShader(s_GEOMETRY_PASS_SHADER, ctx);
        shaderProgram.shader->bind();
        m_ModelData->bind(2);
        const auto& meshes = m_RenderGraphData.geometryStore->getMeshHandles();
        for (const auto& mesh : meshes) {
            m_ModelData->setData(mesh.getInstanceToTransform().getData().data(), sizeof(TransformData) * SR_MAX_INSTANCES);
            mesh.drawMesh(ctx);
        }
    }

    void RenderGraphLayer::onResize(u32 width, u32 height, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::createMesh(const MeshID meshID, const Mesh &mesh, const ResourceView<Context> &ctx) {
        SR_PRECONDITION(m_RenderGraphData.geometryStore != nullptr, "Geometry store is not initialized");

        m_RenderGraphData.geometryStore->createMesh(meshID, mesh, ctx);
    }

    void RenderGraphLayer::createInstance(const InstanceID instanceID, const MeshID meshID, const ResourceView<Context> &ctx) {
        SR_PRECONDITION(m_RenderGraphData.geometryStore != nullptr, "Geometry store is not initialized");

        m_RenderGraphData.geometryStore->createInstance(instanceID, meshID, ctx);
    }


    void RenderGraphLayer::destroyMesh(const MeshID meshID, const ResourceView<Context> &ctx) {
        SR_PRECONDITION(m_RenderGraphData.geometryStore != nullptr, "Geometry store is not initialized");

        m_RenderGraphData.geometryStore->destroyMesh(meshID, ctx);
    }

    void RenderGraphLayer::destroyInstance(const InstanceID instanceID) {
        SR_PRECONDITION(m_RenderGraphData.geometryStore != nullptr, "Geometry store is not initialized");

        m_RenderGraphData.geometryStore->destroyInstance(instanceID);
    }

    void RenderGraphLayer::setInstanceTransform(MeshID meshID, const Transform &transform, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::createCamera(CameraID cameraID, const Camera &camera, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::updateCamera(CameraID cameraID, const Camera &camera, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::destroyCamera(CameraID cameraID, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::createLight(LightID lightID, const Light &light, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::updateLight(LightID lightID, const Light &light, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::destroyLight(LightID lightID, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::createProjection(ProjectionID projectionID, const Projection &projection, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::updateProjection(ProjectionID projectionID, const Projection &projection, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::destroyProjection(ProjectionID projectionID, const ResourceView<Context> &ctx) {

    }


}
