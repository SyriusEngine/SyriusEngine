#include "RenderGraphLayer.hpp"

namespace Syrius{

    RenderGraphLayer::RenderGraphLayer(Resource<ShaderLibrary> &shaderLibrary):
    RenderLayer(),
    RenderCommand(),
    m_ShaderLibrary(shaderLibrary){

    }

    RenderGraphLayer::~RenderGraphLayer() = default;

    void RenderGraphLayer::onAttach() {

    }

    void RenderGraphLayer::onDetach() {

    }

    void RenderGraphLayer::onUpdate() {

    }

    bool RenderGraphLayer::onEvent(const Event &event) {
        return true;
    }

    void RenderGraphLayer::onRendererAttach(ResourceView<Context> &context) {
        // Create resources that depend on the context here
        m_RenderGraph = createResource<RenderGraph>();

        ProjectionPassDesc prpDesc;
        prpDesc.slot = 1;
        prpDesc.bufferName = "ProjectionData";
        prpDesc.projection.windowWidth = static_cast<float>(context->getWidth());
        prpDesc.projection.windowHeight = static_cast<float>(context->getHeight());
        m_RenderGraph->addPass<ProjectionPass>(context, prpDesc);

        CameraDataPassDesc cdpDesc;
        cdpDesc.slot = 0;
        cdpDesc.bufferName = "CameraData";
        m_RenderGraph->addPass<CameraDataPass>(context, cdpDesc);

        GeometryPassDesc gpDesc;
        gpDesc.modelDataBufferSlot = 2;
        gpDesc.modelDataBufferName = "ModelData";
        m_RenderGraph->addPass<GeometryPass>(context, m_ShaderLibrary, gpDesc);

        LightDataPassDesc ldpDesc;
        ldpDesc.slot = 3;
        m_RenderGraph->addPass<LightDataPass>(context, ldpDesc);

        m_RenderGraph->addPass<LFWRSamplerPass>(context, 0);
        m_RenderGraph->addPass<GBufferPass>(context);
        m_RenderGraph->addPass<LightPass>(context, m_ShaderLibrary);

        m_RenderGraph->validate();
        m_RenderGraph->compile();
        m_RenderGraph->generateDot("RG-PBR.dot");
    }

    void RenderGraphLayer::onRendererDetach(ResourceView<Context> &context) {
        // Destroy resources that depend on the context here, not in the destructor

        m_RenderGraph.reset();
    }

    void RenderGraphLayer::onRender(ResourceView<Context> &context) {
        m_RenderGraph->execute();
    }

    void RenderGraphLayer::onResize(uint32_t width, uint32_t height) {
        SR_PRECONDITION(m_RenderGraph->hasPass<ProjectionPass>(), "Projection pass does not exist in the render graph");
        SR_PRECONDITION(m_RenderGraph->hasPass<GBufferPass>(), "GBuffer pass does not exist in the render graph");

        m_RenderGraph->getPass<ProjectionPass>()->onResize(width, height);
        m_RenderGraph->getPass<GBufferPass>()->onResize(width, height);
    }

    void RenderGraphLayer::setProjectionFOV(float fov) {
        SR_PRECONDITION(m_RenderGraph->hasPass<ProjectionPass>(), "Projection pass does not exist in the render graph");

        m_RenderGraph->getPass<ProjectionPass>()->setFOV(fov);
    }

    void RenderGraphLayer::setPlane(float near, float far) {
        SR_PRECONDITION(m_RenderGraph->hasPass<ProjectionPass>(), "Projection pass does not exist in the render graph");

        m_RenderGraph->getPass<ProjectionPass>()->setPlane(near, far);
    }

    MeshID RenderGraphLayer::createMesh(const MeshDesc &meshDesc) {
        SR_PRECONDITION(m_RenderGraph->hasPass<GeometryPass>(), "Geometry pass does not exist in the render graph");

        return m_RenderGraph->getPass<GeometryPass>()->createMesh(meshDesc);
    }

    MeshID RenderGraphLayer::createMesh(MeshID meshID) {
        SR_PRECONDITION(m_RenderGraph->hasPass<GeometryPass>(), "Geometry pass does not exist in the render graph");

        return m_RenderGraph->getPass<GeometryPass>()->createMesh(meshID);
    }

    void RenderGraphLayer::transformMesh(MeshID mesh, const glm::mat4 &transform) {
        SR_PRECONDITION(m_RenderGraph->hasPass<GeometryPass>(), "Geometry pass does not exist in the render graph");

        m_RenderGraph->getPass<GeometryPass>()->transformMesh(mesh, transform);
    }

    void RenderGraphLayer::removeMesh(MeshID mesh) {
        SR_PRECONDITION(m_RenderGraph->hasPass<GeometryPass>(), "Geometry pass does not exist in the render graph");

        m_RenderGraph->getPass<GeometryPass>()->removeMesh(mesh);
    }

    void RenderGraphLayer::updateCamera(const glm::mat4 &viewMat, const glm::vec3 &camPos) {
        SR_PRECONDITION(m_RenderGraph->hasPass<CameraDataPass>(), "CameraData pass does not exist in the render graph");

        m_RenderGraph->getPass<CameraDataPass>()->setCameraData(viewMat, camPos);
    }

    MaterialID RenderGraphLayer::createMaterial(const MaterialDesc &material) {
        SR_PRECONDITION(m_RenderGraph->hasPass<GeometryPass>(), "Geometry pass does not exist in the render graph");

        return m_RenderGraph->getPass<GeometryPass>()->createMaterial(material);
    }

    void RenderGraphLayer::meshSetMaterial(MeshID meshID, MaterialID materialID) {
        SR_PRECONDITION(m_RenderGraph->hasPass<GeometryPass>(), "Geometry pass does not exist in the render graph");

        m_RenderGraph->getPass<GeometryPass>()->setMeshMaterial(meshID, materialID);
    }

    void RenderGraphLayer::removeMaterial(MaterialID materialID) {
        SR_PRECONDITION(m_RenderGraph->hasPass<GeometryPass>(), "Geometry pass does not exist in the render graph");

        m_RenderGraph->getPass<GeometryPass>()->removeMaterial(materialID);
    }

    LightID RenderGraphLayer::createLight(const Light &light) {
        SR_PRECONDITION(m_RenderGraph->hasPass<LightDataPass>(), "LightData pass does not exist in the render graph");

        return m_RenderGraph->getPass<LightDataPass>()->createLight(light);
    }

    void RenderGraphLayer::updateLight(LightID lightID, const Light &light) {
        SR_PRECONDITION(m_RenderGraph->hasPass<LightDataPass>(), "LightData pass does not exist in the render graph");

        m_RenderGraph->getPass<LightDataPass>()->updateLight(lightID, light);
    }

    void RenderGraphLayer::removeLight(LightID lightID) {
        SR_PRECONDITION(m_RenderGraph->hasPass<LightDataPass>(), "LightData pass does not exist in the render graph");

        m_RenderGraph->getPass<LightDataPass>()->removeLight(lightID);
    }
}