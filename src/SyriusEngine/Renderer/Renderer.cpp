#include "Renderer.hpp"

namespace Syrius{

    Renderer::Renderer(const RendererDesc &desc, LayerStack &layerStack, const Resource<SyriusWindow> &window):
    RenderCommand(),
    m_LayerStack(layerStack),
    m_Window(window){
        SR_PRECONDITION(std::filesystem::exists(desc.shaderLibraryPath), "ShaderLibraryPath %s does not exist", desc.shaderLibraryPath.c_str());

        m_RenderThread.addTaskSync([this, &desc]{
            ContextDesc contextDesc;
            contextDesc.api = desc.graphicsAPI;
            contextDesc.enableDepthTest = true;
            m_Context = m_Window->createContext(contextDesc);
            m_Context->setVerticalSynchronisation(desc.vSync);

            m_Context->setClearColor(desc.clearColor[0],
                                     desc.clearColor[1],
                                     desc.clearColor[2],
                                     1.0f);

            m_ShaderLibrary = createResource<ShaderLibrary>(m_Context, desc.shaderLibraryPath, desc.enableExperimentalSRSLShaderCompiler);
        });

        auto pbrRenderLayer = createResource<PBRRenderLayer>(m_ShaderLibrary);
        m_PBRLayer = createResourceView(pbrRenderLayer);
        pushRenderLayer(std::move(pbrRenderLayer));

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
            m_Context->clear();

            m_LayerStack.onRender(m_Context);
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
        SR_PRECONDITION(m_PBRLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_PBRLayer.get());

        m_RenderThread.addTask([this, width, height]{
            m_Context->onResize(width, height);

            m_PBRLayer->onResize(width, height);
        });
    }

    void Renderer::setProjectionFOV(float fov) {
        SR_PRECONDITION(m_PBRLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_PBRLayer.get());

        m_RenderThread.addTask([this, fov]{
            m_PBRLayer->setProjectionFOV(fov);
        });
    }

    void Renderer::setPlane(float near, float far) {
        SR_PRECONDITION(m_PBRLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_PBRLayer.get());

        m_RenderThread.addTask([this, near, far]{
            m_PBRLayer->setPlane(near, far);
        });
    }

    MeshID Renderer::createMesh(const MeshDesc &meshDesc) {
        SR_PRECONDITION(m_PBRLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_PBRLayer.get());

        MeshID meshID = 0;
        // wait for the render thread to create the mesh, otherwise invalid meshIDs may be used
        m_RenderThread.addTaskSync([this, &meshDesc, &meshID]{
            meshID = m_PBRLayer->createMesh(meshDesc);
        });
        return meshID;
    }

    void Renderer::transformMesh(MeshID mesh, const glm::mat4 &transform) {
        SR_PRECONDITION(m_PBRLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_PBRLayer.get());
        SR_PRECONDITION(mesh != 0, "MeshID is 0 (%d)", mesh);

        m_RenderThread.addTask([this, mesh, transform]{
            m_PBRLayer->transformMesh(mesh, transform);
        });
    }

    void Renderer::removeMesh(MeshID mesh) {
        SR_PRECONDITION(m_PBRLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_PBRLayer.get());
        SR_PRECONDITION(mesh != 0, "MeshID is 0 (%d)", mesh);

        m_RenderThread.addTask([this, mesh]{
            m_PBRLayer->removeMesh(mesh);
        });
    }

    void Renderer::updateCamera(const glm::mat4 &viewMat, const glm::vec3 &camPos) {
        SR_PRECONDITION(m_PBRLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_PBRLayer.get());

        m_RenderThread.addTask([this, viewMat, camPos]{
            m_PBRLayer->updateCamera(viewMat, camPos);
        });
    }

    MaterialID Renderer::createMaterial(const MaterialDesc &material) {
        SR_PRECONDITION(m_PBRLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_PBRLayer.get());

        MaterialID materialID = 0;
        m_RenderThread.addTaskSync([this, &material, &materialID]{
            materialID = m_PBRLayer->createMaterial(material);
        });

        SR_POSTCONDITION(materialID != 0, "Material ID is %i", materialID);
        return materialID;
    }

    void Renderer::meshSetMaterial(MeshID meshID, MaterialID materialID) {
        SR_PRECONDITION(m_PBRLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_PBRLayer.get());

        m_RenderThread.addTask([this, meshID, materialID]{
            m_PBRLayer->meshSetMaterial(meshID, materialID);
        });
    }

    void Renderer::removeMaterial(MaterialID materialID) {
        SR_PRECONDITION(m_PBRLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_PBRLayer.get());
        SR_PRECONDITION(materialID != 0, "Material ID is %i", materialID);

        m_RenderThread.addTask([this, materialID]{
            m_PBRLayer->removeMaterial(materialID);
        });

    }

    LightID Renderer::createLight(const Light &light) {
        SR_PRECONDITION(m_PBRLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_PBRLayer.get());

        LightID lightID = 0;
        m_RenderThread.addTaskSync([this, &light, &lightID]{
            lightID = m_PBRLayer->createLight(light);
        });

        SR_POSTCONDITION(lightID != 0, "Light ID is %i", lightID);
        return lightID;
    }

    void Renderer::updateLight(LightID lightID, const Light &light) {
        SR_PRECONDITION(m_PBRLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_PBRLayer.get());

        m_RenderThread.addTask([this, lightID, light]{
            m_PBRLayer->updateLight(lightID, light);
        });
    }

    void Renderer::removeLight(LightID lightID) {
        SR_PRECONDITION(m_PBRLayer.get() != nullptr, "PBRRenderLayer is null (%p)", m_PBRLayer.get());

        m_RenderThread.addTask([this, lightID]{
            m_PBRLayer->removeLight(lightID);
        });
    }

}
