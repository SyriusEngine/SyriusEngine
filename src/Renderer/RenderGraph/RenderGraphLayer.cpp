#include "RenderGraphLayer.hpp"

#include "RenderGraphDefs.hpp"

namespace Syrius::Renderer {

    RenderGraphLayer::RenderGraphLayer(const fs::path &shaderPath) {
        m_RenderGraphData.shaderStore = createUP<ShaderStore>(shaderPath);
    }

    void RenderGraphLayer::onRendererAttach(const ResourceView<Context> &ctx) {
        SR_PRECONDITION(m_RenderGraphData.shaderStore != nullptr, "Shader store is not initialized");

        m_RenderGraphData.geometryStore = createUP<GeometryStore>(m_RenderGraphData.shaderStore, ctx);

        m_RenderGraphData.transformHandle = createUP<TransformHandle>(ctx);
        m_RenderGraphData.projectionHandle = createUP<ProjectionHandle>(ctx);
        m_RenderGraphData.cameraHandle = createUP<CameraHandle>(ctx);
        m_RenderGraphData.samplerHandle = createUP<SamplerHandle>(ctx);
        m_RenderGraphData.gBufferHandle = createUP<GBufferHandle>(ctx);

        createPNRRenderGraph();
    }

    void RenderGraphLayer::onRendererDetach(const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::onRender(const ResourceView<Context> &ctx) {
        m_RenderGraph.execute(m_RenderGraphData, ctx);
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
        m_RenderGraphData.instanceToMeshID.emplace(instanceID, meshID);
    }


    void RenderGraphLayer::destroyMesh(const MeshID meshID, const ResourceView<Context> &ctx) {
        SR_PRECONDITION(m_RenderGraphData.geometryStore != nullptr, "Geometry store is not initialized");

        m_RenderGraphData.geometryStore->destroyMesh(meshID, ctx);
    }

    void RenderGraphLayer::destroyInstance(const InstanceID instanceID, const ResourceView<Context>& ctx) {
        SR_PRECONDITION(m_RenderGraphData.geometryStore != nullptr, "Geometry store is not initialized");

        m_RenderGraphData.geometryStore->destroyInstance(instanceID);
        auto it = m_RenderGraphData.instanceToMeshID.find(instanceID);
        if (it != m_RenderGraphData.instanceToMeshID.end()) {
            m_RenderGraphData.instanceToMeshID.erase(it);
        } else {
            SR_LOG_WARNING("RenderGraphLayer", "Instance {} does not exist!", instanceID);
        }
    }

    void RenderGraphLayer::setInstanceTransform(const InstanceID instanceID, const Transform &transform, const ResourceView<Context> &ctx) {
        SR_PRECONDITION(m_RenderGraphData.geometryStore != nullptr, "Geometry store is not initialized");

        MeshID meshID = m_RenderGraphData.instanceToMeshID.at(instanceID);
        auto& meshHandles = m_RenderGraphData.geometryStore->getMeshHandles();
        meshHandles[meshID].setTransformation(instanceID, transform, ctx);
    }

    void RenderGraphLayer::setCamera(const CameraID cameraID, const Camera &camera, const ResourceView<Context> &ctx) {
        SR_PRECONDITION(m_RenderGraphData.cameraHandle != nullptr, "Camera handle is not initialized");

        m_RenderGraphData.cameraHandle->setCamera(cameraID, camera);
    }

    void RenderGraphLayer::createLight(LightID lightID, const Light &light, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::updateLight(LightID lightID, const Light &light, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::destroyLight(LightID lightID, const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::setProjection(ProjectionID projectionID, const Projection &projection, const ResourceView<Context> &ctx) {
        SR_PRECONDITION(m_RenderGraphData.projectionHandle != nullptr, "Projection handle is not initialized");

        m_RenderGraphData.projectionHandle->updateProjection(projectionID, projection);
    }

    void RenderGraphLayer::createPNRRenderGraph() {
        RenderGraphNode transformNode = {
            {},
            {SR_NODE_TRANSFORM_DATA},
            [](const ResourceView<Context>& ctx, const RenderGraphData& graphData) {
                graphData.transformHandle->bind(2);
            }
        };
        m_RenderGraph.addNode(transformNode);

        RenderGraphNode projectionNode = {
            {},
            {SR_NODE_PROJECTION_DATA},
            [](const ResourceView<Context>& ctx, const RenderGraphData& graphData) {
                graphData.projectionHandle->bind(1);
            }
        };
        m_RenderGraph.addNode(projectionNode);

        RenderGraphNode cameraNode = {
            {},
            {SR_NODE_CAMERA_DATA},
            [](const ResourceView<Context>& ctx, const RenderGraphData& graphData) {
                graphData.cameraHandle->bind(0);
            }
        };
        m_RenderGraph.addNode(cameraNode);

        RenderGraphNode samplerNode = {
            {},
            {SR_NODE_SAMPLER_DATA},
            [](const ResourceView<Context>& ctx, const RenderGraphData& graphData) {
                graphData.samplerHandle->bind(0);
            }
        };

        RenderGraphNode geometryPass = {
            {SR_NODE_TRANSFORM_DATA, SR_NODE_CAMERA_DATA, SR_NODE_PROJECTION_DATA},
            {SR_NODE_DRAW_GEOMETRY},
            [](const ResourceView<Context>& ctx, const RenderGraphData & graphData) {
                const ShaderProgram& shaderProgram = graphData.shaderStore->getShader(s_GEOMETRY_PASS_SHADER, ctx);
                shaderProgram.shader->bind();
                const auto& meshes = graphData.geometryStore->getMeshHandles();
                for (const auto& mesh : meshes) {
                    graphData.transformHandle->setData(mesh.getInstanceToTransform());
                    mesh.drawMesh(ctx);
                }
            }
        };
        m_RenderGraph.addNode(geometryPass);

        if (!m_RenderGraph.validate()) {
            SR_LOG_THROW("RenderGraphLayer", "Render graph is invalid");
        }
        m_RenderGraph.compile();
        m_RenderGraph.generateDot();
    }
}
