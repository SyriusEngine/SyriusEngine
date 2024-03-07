#include "PBRRenderLayer.hpp"

namespace Syrius{

    PBRRenderLayer::PBRRenderLayer(Resource<ShaderLibrary> &shaderLibrary):
    RenderLayer(),
    RenderCommand(),
    m_ShaderLibrary(shaderLibrary),
    m_ProjectionPass(nullptr),
    m_CameraDataPass(nullptr){

    }

    PBRRenderLayer::~PBRRenderLayer() = default;

    void PBRRenderLayer::onAttach() {

    }

    void PBRRenderLayer::onDetach() {

    }

    void PBRRenderLayer::onUpdate() {

    }

    bool PBRRenderLayer::onEvent(const Event &event) {
        return true;
    }

    void PBRRenderLayer::onRendererAttach(ResourceView<Context> &context) {
        // Create resources that depend on the context here
        m_RenderGraph = createResource<RenderGraph>();

        ProjectionPassDesc prpDesc;
        prpDesc.slot = 1;
        prpDesc.bufferName = "ProjectionData";
        prpDesc.projection.windowWidth = static_cast<float>(context->getWidth());
        prpDesc.projection.windowHeight = static_cast<float>(context->getHeight());
        m_ProjectionPass = m_RenderGraph->addPass<ProjectionPass>(context, prpDesc);

        CameraDataPassDesc cdpDesc;
        cdpDesc.slot = 0;
        cdpDesc.bufferName = "CameraData";
        m_CameraDataPass = m_RenderGraph->addPass<CameraDataPass>(context, cdpDesc);

        GeometryPassDesc gpDesc;
        gpDesc.modelDataBufferSlot = 2;
        gpDesc.modelDataBufferName = "ModelData";
        m_GeometryPass = m_RenderGraph->addPass<GeometryPass>(context, m_ShaderLibrary, gpDesc);

        LightDataPassDesc ldpDesc;
        ldpDesc.slot = 3;
        m_LightDataPass = m_RenderGraph->addPass<LightDataPass>(context, ldpDesc);

        m_RenderGraph->addPass<LFWRSamplerPass>(context, 0);
        m_RenderGraph->addPass<GBufferPass>(context);
        m_RenderGraph->addPass<LightPass>(context, m_ShaderLibrary);

        m_RenderGraph->validate();
        m_RenderGraph->compile();
        m_RenderGraph->generateDot("RG-PBR.dot");
    }

    void PBRRenderLayer::onRendererDetach(ResourceView<Context> &context) {
        // Destroy resources that depend on the context here, not in the destructor

        m_RenderGraph.reset();
    }

    void PBRRenderLayer::onRender(ResourceView<Context> &context) {
        m_RenderGraph->execute();
    }

    void PBRRenderLayer::onResize(uint32_t width, uint32_t height) {
        SR_PRECONDITION(m_ProjectionPass != nullptr, "Projection pass is null (%p)", m_ProjectionPass);

        m_ProjectionPass->onResize(width, height);
    }

    void PBRRenderLayer::setProjectionFOV(float fov) {
        SR_PRECONDITION(m_ProjectionPass != nullptr, "Projection pass is null (%p)", m_ProjectionPass);

        m_ProjectionPass->setFOV(fov);
    }

    void PBRRenderLayer::setPlane(float near, float far) {
        SR_PRECONDITION(m_ProjectionPass != nullptr, "Projection pass is null (%p)", m_ProjectionPass);

        m_ProjectionPass->setPlane(near, far);
    }

    MeshID PBRRenderLayer::createMesh(const MeshDesc &meshDesc) {
        SR_PRECONDITION(m_GeometryPass != nullptr, "GeometryPass is null (%p)", m_GeometryPass);

        return m_GeometryPass->createMesh(meshDesc);
    }

    void PBRRenderLayer::transformMesh(MeshID mesh, const glm::mat4 &transform) {
        SR_PRECONDITION(m_GeometryPass != nullptr, "GeometryPass is null (%p)", m_GeometryPass);

        m_GeometryPass->transformMesh(mesh, transform);
    }

    void PBRRenderLayer::removeMesh(MeshID mesh) {
        SR_PRECONDITION(m_GeometryPass != nullptr, "GeometryPass is null (%p)", m_GeometryPass);

        m_GeometryPass->removeMesh(mesh);
    }

    void PBRRenderLayer::updateCamera(const glm::mat4 &viewMat, const glm::vec3 &camPos) {
        SR_PRECONDITION(m_CameraDataPass != nullptr, "CameraDataPass is null (%p)", m_CameraDataPass);

        m_CameraDataPass->setCameraData(viewMat, camPos);
    }

    MaterialID PBRRenderLayer::createMaterial(const MaterialDesc &material) {
        SR_PRECONDITION(m_GeometryPass != nullptr, "GeometryPass is null (%p)", m_GeometryPass);

        return m_GeometryPass->createMaterial(material);
    }

    void PBRRenderLayer::meshSetMaterial(MeshID meshID, MaterialID materialID) {
        SR_PRECONDITION(m_GeometryPass != nullptr, "GeometryPass is null (%p)", m_GeometryPass);

        m_GeometryPass->setMeshMaterial(meshID, materialID);
    }

    void PBRRenderLayer::removeMaterial(MaterialID materialID) {
        SR_PRECONDITION(m_GeometryPass != nullptr, "GeometryPass is null (%p)", m_GeometryPass);

        m_GeometryPass->removeMaterial(materialID);
    }

    LightID PBRRenderLayer::createLight(const Light &light) {
        SR_PRECONDITION(m_LightDataPass != nullptr, "LightDataPass is null (%p)", m_LightDataPass);

        return m_LightDataPass->createLight(light);
    }

    void PBRRenderLayer::updateLight(LightID lightID, const Light &light) {
        SR_PRECONDITION(m_LightDataPass != nullptr, "LightDataPass is null (%p)", m_LightDataPass);

        m_LightDataPass->updateLight(lightID, light);
    }

    void PBRRenderLayer::removeLight(LightID lightID) {
        SR_PRECONDITION(m_LightDataPass != nullptr, "LightDataPass is null (%p)", m_LightDataPass);

        m_LightDataPass->removeLight(lightID);
    }
}