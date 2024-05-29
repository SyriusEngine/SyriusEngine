#include "Renderer.hpp"

namespace Syrius{

    Renderer::Renderer(const RendererDesc &desc, LayerStack &layerStack, const Resource<SyriusWindow> &window):
    RenderCommand(),
    m_RenderThread("RenderThread"),
    m_LayerStack(layerStack),
    m_Window(window){
        SR_PRECONDITION(std::filesystem::exists(desc.shaderLibraryPath), "ShaderLibraryPath %s does not exist", desc.shaderLibraryPath.c_str());

        SR_START_TIMER("Renderer::Setup")
        m_RenderThread.addTaskSync([this, &desc]{
            ContextDesc contextDesc;
            contextDesc.api = desc.graphicsAPI;
            contextDesc.enableDepthTest = false;
            m_Context = m_Window->createContext(contextDesc);
            m_Context->setVerticalSynchronisation(desc.vSync);

            m_Context->setClearColor(desc.clearColor[0],
                                     desc.clearColor[1],
                                     desc.clearColor[2],
                                     1.0f);

            m_ShaderLibrary = createResource<ShaderLibrary>(m_Context, desc.shaderLibraryPath, desc.enableExperimentalSRSLShaderCompiler);
        });

        auto pbrRenderLayer = createResource<RenderGraphLayer>(m_ShaderLibrary);
        m_ActiveLayer = createResourceView(pbrRenderLayer);
        pushRenderLayer(std::move(pbrRenderLayer));

        SR_STOP_TIMER("Renderer::Setup")
    }

    Renderer::~Renderer() {
        m_RenderThread.addTaskSync([this]{
            m_LayerStack.detachRenderLayers(m_Context);
            m_Window->destroyContext();
        });
        m_RenderThread.stop();
    }

    void Renderer::render() {
        m_RenderThread.addTask([this]{
            SR_START_TIMER("Renderer::Render")
            m_Context->clear();

            m_LayerStack.onRender(m_Context);
            SR_STOP_TIMER("Renderer::Render")
        });
    }

    void Renderer::swapBuffers() {
        m_RenderThread.addTaskSync([this]{
            m_Context->swapBuffers();
        });
    }

    void Renderer::pushRenderLayer(Resource<RenderLayer> layer) {
        // first push the layer onto the normal layer stack
        auto layerView = createResourceView(layer);
        m_LayerStack.pushLayer(std::move(layer));

        // then also add a ptr to the layer on the render layer stack
        m_RenderThread.addTask([this, layerView]{
            m_LayerStack.pushRenderLayer(layerView, m_Context);
        });
    }

    void Renderer::onResize(uint32_t width, uint32_t height) {
        SR_PRECONDITION(m_ActiveLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_ActiveLayer.get());

        m_RenderThread.addTask([this, width, height]{
            m_Context->onResize(width, height);

            m_ActiveLayer->onResize(width, height);
        });
    }

    void Renderer::setProjectionFOV(float fov) {
        SR_PRECONDITION(m_ActiveLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_ActiveLayer.get());

        m_RenderThread.addTask([this, fov]{
            m_ActiveLayer->setProjectionFOV(fov);
        });
    }

    void Renderer::setPlane(float near, float far) {
        SR_PRECONDITION(m_ActiveLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_ActiveLayer.get());

        m_RenderThread.addTask([this, near, far]{
            m_ActiveLayer->setPlane(near, far);
        });
    }

    MeshID Renderer::createMesh(const MeshDesc &meshDesc) {
        SR_PRECONDITION(m_ActiveLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_ActiveLayer.get());

        MeshID meshID = 0;
        // wait for the render thread to create the mesh, otherwise invalid meshIDs may be used
        m_RenderThread.addTaskSync([this, &meshDesc, &meshID]{
            meshID = m_ActiveLayer->createMesh(meshDesc);
        });
        return meshID;
    }

    MeshID Renderer::createMesh(MeshID meshID) {
        SR_PRECONDITION(m_ActiveLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_ActiveLayer.get());

        MeshID newID = 0;
        // wait for the render thread to create the mesh, otherwise invalid meshIDs may be used
        m_RenderThread.addTaskSync([this, meshID, &newID]{
            newID = m_ActiveLayer->createMesh(meshID);
        });
        return newID;
    }

    void Renderer::transformMesh(MeshID mesh, const glm::mat4 &transform) {
        SR_PRECONDITION(m_ActiveLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_ActiveLayer.get());
        SR_PRECONDITION(mesh != 0, "MeshID is 0 (%d)", mesh);

        m_RenderThread.addTask([this, mesh, transform]{
            m_ActiveLayer->transformMesh(mesh, transform);
        });
    }

    void Renderer::removeMesh(MeshID mesh) {
        SR_PRECONDITION(m_ActiveLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_ActiveLayer.get());
        SR_PRECONDITION(mesh != 0, "MeshID is 0 (%d)", mesh);

        m_RenderThread.addTask([this, mesh]{
            m_ActiveLayer->removeMesh(mesh);
        });
    }

    void Renderer::updateCamera(const glm::mat4 &viewMat, const glm::vec3 &camPos) {
        SR_PRECONDITION(m_ActiveLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_ActiveLayer.get());

        m_RenderThread.addTask([this, viewMat, camPos]{
            m_ActiveLayer->updateCamera(viewMat, camPos);
        });
    }

    MaterialID Renderer::createMaterial(const MaterialDesc &material) {
        SR_PRECONDITION(m_ActiveLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_ActiveLayer.get());

        MaterialID materialID = 0;
        m_RenderThread.addTaskSync([this, &material, &materialID]{
            materialID = m_ActiveLayer->createMaterial(material);
        });

        SR_POSTCONDITION(materialID != 0, "Material ID is %i", materialID);
        return materialID;
    }

    void Renderer::meshSetMaterial(MeshID meshID, MaterialID materialID) {
        SR_PRECONDITION(m_ActiveLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_ActiveLayer.get());

        m_RenderThread.addTask([this, meshID, materialID]{
            m_ActiveLayer->meshSetMaterial(meshID, materialID);
        });
    }

    void Renderer::removeMaterial(MaterialID materialID) {
        SR_PRECONDITION(m_ActiveLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_ActiveLayer.get());
        SR_PRECONDITION(materialID != 0, "Material ID is %i", materialID);

        m_RenderThread.addTask([this, materialID]{
            m_ActiveLayer->removeMaterial(materialID);
        });

    }

    LightID Renderer::createLight(const Light &light) {
        SR_PRECONDITION(m_ActiveLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_ActiveLayer.get());

        LightID lightID = 0;
        m_RenderThread.addTaskSync([this, &light, &lightID]{
            lightID = m_ActiveLayer->createLight(light);
        });

        SR_POSTCONDITION(lightID != 0, "Light ID is %i", lightID);
        return lightID;
    }

    void Renderer::updateLight(LightID lightID, const Light &light) {
        SR_PRECONDITION(m_ActiveLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_ActiveLayer.get());

        m_RenderThread.addTask([this, lightID, light]{
            m_ActiveLayer->updateLight(lightID, light);
        });
    }

    void Renderer::removeLight(LightID lightID) {
        SR_PRECONDITION(m_ActiveLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_ActiveLayer.get());

        m_RenderThread.addTask([this, lightID]{
            m_ActiveLayer->removeLight(lightID);
        });
    }

    void Renderer::setSkyBox(const Resource<Image> &radianceMap) {
        SR_PRECONDITION(m_ActiveLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_ActiveLayer.get());

        m_RenderThread.addTaskSync([this, &radianceMap]{
            m_ActiveLayer->setSkyBox(radianceMap);
        });

    }

}
