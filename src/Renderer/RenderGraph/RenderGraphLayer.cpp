#include "RenderGraphLayer.hpp"
#include "Handles/CameraHandle.hpp"
#include "Handles/GBufferHandle.hpp"
#include "Handles/MaterialHandle.hpp"
#include "Handles/MeshHandle.hpp"
#include "Handles/ProjectionHandle.hpp"
#include "Handles/SamplerHandle.hpp"
#include "Handles/TransformHandle.hpp"
#include "Other/ScreenQuad.hpp"
#include "RenderGraphDefs.hpp"

#include "Stores/GeometryStore.hpp"
#include "Stores/LightStore.hpp"
#include "Stores/MaterialStore.hpp"
#include "Stores/ShaderStore.hpp"

namespace Syrius::Renderer {

    RenderGraphLayer::RenderGraphLayer(const fs::path &shaderPath, const SP<DispatcherManager> &dispatcherManager):
    m_ShaderPath(shaderPath),
    m_Container(createUP<RenderGraphContainer>()),
    m_DispatcherManager(dispatcherManager) {

    }

    void RenderGraphLayer::onRendererAttach(const ResourceView<Context> &ctx) {
        m_Container->createData<ShaderStore>(ctx, m_Container.get(), m_ShaderPath);
        m_Container->createData<GeometryStore>(ctx, m_Container.get(), m_DispatcherManager);
        m_Container->createData<MaterialStore>(ctx, m_Container.get(), m_DispatcherManager);
        m_Container->createData<LightStore>(ctx, m_Container.get(), m_DispatcherManager);

        m_Container->createData<TransformHandle>(ctx, m_Container.get());
        m_Container->createData<ProjectionHandle>(ctx, m_Container.get(), m_DispatcherManager);
        m_Container->createData<CameraHandle>(ctx, m_Container.get(), m_DispatcherManager);
        m_Container->createData<SamplerHandle>(ctx, m_Container.get());
        m_Container->createData<GBufferHandle>(ctx, m_Container.get());

        m_Container->createData<ScreenQuad>(ctx, m_Container.get());

        createPBRRenderGraph();
    }

    void RenderGraphLayer::onRendererDetach(const ResourceView<Context> &ctx) {

    }

    void RenderGraphLayer::onRender(const ResourceView<Context> &ctx) {
        m_RenderGraph.execute(m_Container.get(), ctx);
    }

    void RenderGraphLayer::onResize(u32 width, u32 height, const ResourceView<Context> &ctx) {

    }

    // void RenderGraphLayer::createMesh(const MeshID meshID, const Mesh &mesh, const ResourceView<Context> &ctx) {
    //     SR_PRECONDITION(m_RenderGraphData.geometryStore != nullptr, "Geometry store is not initialized");
    //
    //     m_RenderGraphData.geometryStore->createMesh(meshID, mesh, ctx);
    // }
    //
    // void RenderGraphLayer::createInstance(const InstanceID instanceID, const MeshID meshID, const ResourceView<Context> &ctx) {
    //     SR_PRECONDITION(m_RenderGraphData.geometryStore != nullptr, "Geometry store is not initialized");
    //
    //     m_RenderGraphData.geometryStore->createInstance(instanceID, meshID, ctx);
    //     m_RenderGraphData.instanceToMeshID.emplace(instanceID, meshID);
    // }
    //
    //
    // void RenderGraphLayer::destroyMesh(const MeshID meshID, const ResourceView<Context> &ctx) {
    //     SR_PRECONDITION(m_RenderGraphData.geometryStore != nullptr, "Geometry store is not initialized");
    //
    //     m_RenderGraphData.geometryStore->destroyMesh(meshID, ctx);
    // }
    //
    // void RenderGraphLayer::destroyInstance(const InstanceID instanceID, const ResourceView<Context>& ctx) {
    //     SR_PRECONDITION(m_RenderGraphData.geometryStore != nullptr, "Geometry store is not initialized");
    //
    //     m_RenderGraphData.geometryStore->destroyInstance(instanceID);
    //     auto it = m_RenderGraphData.instanceToMeshID.find(instanceID);
    //     if (it != m_RenderGraphData.instanceToMeshID.end()) {
    //         m_RenderGraphData.instanceToMeshID.erase(it);
    //     } else {
    //         SR_LOG_WARNING("RenderGraphLayer", "Instance {} does not exist!", instanceID);
    //     }
    // }
    //
    // void RenderGraphLayer::setInstanceTransform(const InstanceID instanceID, const Transform &transform, const ResourceView<Context> &ctx) {
    //     SR_PRECONDITION(m_RenderGraphData.geometryStore != nullptr, "Geometry store is not initialized");
    //
    //     const MeshID meshID = m_RenderGraphData.instanceToMeshID.at(instanceID);
    //     auto& meshHandles = m_RenderGraphData.geometryStore->getMeshHandles();
    //     meshHandles[meshID].setTransformation(instanceID, transform, ctx);
    // }
    //
    // void RenderGraphLayer::setCamera(const CameraID cameraID, const Camera &camera, const ResourceView<Context> &ctx) {
    //     SR_PRECONDITION(m_RenderGraphData.cameraHandle != nullptr, "Camera handle is not initialized");
    //
    //     m_RenderGraphData.cameraHandle->setCamera(cameraID, camera);
    // }
    //
    // void RenderGraphLayer::createMaterial(const MaterialID materialID, const Material &material,
    //                                       const ResourceView<Context> &ctx) {
    //     SR_PRECONDITION(m_RenderGraphData.materialStore != nullptr, "Material store is not initialized");
    //
    //     m_RenderGraphData.materialStore->createMaterial(materialID, material, ctx);
    // }
    //
    // void RenderGraphLayer::setMeshMaterial(const MeshID meshID, const MaterialID materialID, const ResourceView<Context> &ctx) {
    //     SR_PRECONDITION(m_RenderGraphData.geometryStore != nullptr, "Geometry store is not initialized");
    //
    //     m_RenderGraphData.geometryStore->setMeshMaterial(meshID, materialID);
    //     m_RenderGraphData.materialToMesh[materialID].push_back(meshID);
    // }
    //
    // void RenderGraphLayer::destroyMaterial(const MaterialID materialID, const ResourceView<Context> &ctx) {
    //     SR_PRECONDITION(m_RenderGraphData.materialStore != nullptr, "Material store is not initialized");
    //
    //     m_RenderGraphData.materialStore->destroyMaterial(materialID, ctx);
    //
    //     // Some meshes may still be using this material, reset them to the default material
    //     const auto& meshes = m_RenderGraphData.materialToMesh.at(materialID);
    //     for (const MeshID mesh: meshes) {
    //         if (m_RenderGraphData.geometryStore->getMeshHandles().has(mesh)) {
    //             m_RenderGraphData.geometryStore->setMeshMaterial(mesh, SR_DEFAULT_MATERIAL);
    //         }
    //     }
    // }
    //
    // void RenderGraphLayer::createLight(const LightID lightID, const Light &light, const ResourceView<Context> &ctx) {
    //     SR_PRECONDITION(m_RenderGraphData.lightStore != nullptr, "LightStore is not initialized")
    //
    //     m_RenderGraphData.lightStore->createLight(lightID, light);
    //
    // }
    //
    // void RenderGraphLayer::setLight(const LightID lightID, const Light &light, const ResourceView<Context> &ctx) {
    //     SR_PRECONDITION(m_RenderGraphData.lightStore != nullptr, "LightStore is not initialized")
    //
    //     m_RenderGraphData.lightStore->setLight(lightID, light);
    // }
    //
    // void RenderGraphLayer::destroyLight(const LightID lightID, const ResourceView<Context> &ctx) {
    //     SR_PRECONDITION(m_RenderGraphData.lightStore != nullptr, "LightStore is not initialized")
    //
    //     m_RenderGraphData.lightStore->destroyLight(lightID);
    // }
    //
    // void RenderGraphLayer::setProjection(const ProjectionID projectionID, const Projection &projection, const ResourceView<Context> &ctx) {
    //     SR_PRECONDITION(m_RenderGraphData.projectionHandle != nullptr, "Projection handle is not initialized");
    //
    //     m_RenderGraphData.projectionHandle->updateProjection(projectionID, projection);
    // }

    void RenderGraphLayer::createPBRRenderGraph() {
        RenderGraphNode transformNode = {
            {},
            {SR_NODE_TRANSFORM_DATA},
            [](const ResourceView<Context>& ctx, RenderGraphContainer* graphData) {
                const auto transformHandle = graphData->getData<TransformHandle>();
                transformHandle->bind(2);
            }
        };
        m_RenderGraph.addNode(transformNode);

        RenderGraphNode projectionNode = {
            {},
            {SR_NODE_PROJECTION_DATA},
            [](const ResourceView<Context>& ctx, RenderGraphContainer* graphData) {
                const auto projectionHandle = graphData->getData<ProjectionHandle>();
                projectionHandle->bind(1);
            }
        };
        m_RenderGraph.addNode(projectionNode);

        RenderGraphNode cameraNode = {
            {},
            {SR_NODE_CAMERA_DATA},
            [](const ResourceView<Context>& ctx, RenderGraphContainer* graphData) {
                const auto cameraHandle = graphData->getData<CameraHandle>();
                cameraHandle->bind(0);
            }
        };
        m_RenderGraph.addNode(cameraNode);

        RenderGraphNode samplerNode = {
            {},
            {SR_NODE_SAMPLER_DATA},
            [](const ResourceView<Context>& ctx, RenderGraphContainer* graphData) {
                const auto samplerHandle = graphData->getData<SamplerHandle>();
                samplerHandle->bind(0);
            }
        };
        m_RenderGraph.addNode(samplerNode);

        RenderGraphNode clearGBufferNode = {
            {},
            {SR_NODE_CLEAR_GBUFFER},
            [](const ResourceView<Context>& ctx, RenderGraphContainer* graphData) {
                const auto gBufferHandle = graphData->getData<GBufferHandle>();
                gBufferHandle->clear();
            }
        };
        m_RenderGraph.addNode(clearGBufferNode);

        RenderGraphNode geometryPass = {
            {SR_NODE_TRANSFORM_DATA, SR_NODE_CAMERA_DATA, SR_NODE_PROJECTION_DATA, SR_NODE_CLEAR_GBUFFER, SR_NODE_SAMPLER_DATA},
            {SR_NODE_DRAW_GEOMETRY, SR_NODE_DRAW_GBUFFER},
            [](const ResourceView<Context>& ctx, RenderGraphContainer* graphData) {
                // Bind geometry pass shader
                const auto shaderStore = graphData->getData<ShaderStore>();
                const ShaderProgram& shaderProgram = shaderStore->getShader(s_GEOMETRY_PASS_SHADER);
                shaderProgram.shader->bind();

                const auto gBufferHandle = graphData->getData<GBufferHandle>();
                gBufferHandle->beginRenderPass(); // Instruct GBuffer we will draw to this buffer

                // draw the geometry
                const auto geometryStore = graphData->getData<GeometryStore>();
                const auto transformHandle = graphData->getData<TransformHandle>();
                const auto materialStore = graphData->getData<MaterialStore>();
                const auto& meshes = geometryStore->getMeshHandles();
                for (const auto& mesh : meshes) {
                    transformHandle->setData(mesh.getInstanceToTransform());
                    materialStore->bindMeshMaterial(mesh.getMeshID(), 0);
                    mesh.drawMesh(ctx);
                }
                gBufferHandle->endRenderPass(); // We are done drawing to the gbuffer
            }
        };
        m_RenderGraph.addNode(geometryPass);

        RenderGraphNode lightDataNode = {
            {},
            {SR_NODE_LIGHT_DATA},
            [](const ResourceView<Context>& ctx, RenderGraphContainer* graphData) {
                const auto lightStore = graphData->getData<LightStore>();
                lightStore->bind(3);
            }
        };
        m_RenderGraph.addNode(lightDataNode);

        RenderGraphNode lightPass = {
            {SR_NODE_DRAW_GBUFFER, SR_NODE_LIGHT_DATA},
            {SR_NODE_DRAW_LIGHTS},
            [](const ResourceView<Context>& ctx, RenderGraphContainer* graphData) {

                // Bind light pass shader
                const auto shaderStore = graphData->getData<ShaderStore>();
                const auto gBufferHandle = graphData->getData<GBufferHandle>();
                const ShaderProgram& shaderProgram = shaderStore->getShader(s_LIGHT_PASS_SHADER);
                shaderProgram.shader->bind();
                gBufferHandle->bind(0);

                // For now, draw to the default framebuffer
                ctx->getDefaultFrameBuffer()->bind();

                const auto screenQuad = graphData->getData<ScreenQuad>();
                screenQuad->draw(ctx);
            }
        };
        m_RenderGraph.addNode(lightPass);

        if (!m_RenderGraph.validate()) {
            SR_LOG_THROW("RenderGraphLayer", "Render graph is invalid");
        }
        m_RenderGraph.compile();
        m_RenderGraph.generateDot();
    }
}
