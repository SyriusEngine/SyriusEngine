#include "PBRRenderLayer.hpp"

namespace Syrius{

    PBRRenderLayer::PBRRenderLayer(Resource<ShaderLibrary> &shaderLibrary):
    RenderLayer(),
    RenderCommand(),
    m_ShaderLibrary(shaderLibrary){

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
        return 0;
    }

    void PBRRenderLayer::transformMesh(MeshID mesh, const glm::mat4 &transform) {

    }

    void PBRRenderLayer::removeMesh(MeshID mesh) {

    }
}