#include "Renderer.hpp"

namespace Syrius{

    Renderer::Renderer(const RendererDesc &desc, LayerStack &layerStack, const Resource<SyriusWindow> &window):
    RenderCommand(),
    m_LayerStack(layerStack),
    m_Window(window){
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
        });

    }

    Renderer::~Renderer() {
        m_RenderThread.addTaskSync([this]{
            m_LayerStack.detachRenderLayers(m_Context);
            m_Window->destroyContext();
        });
        m_RenderThread.stop();
    }

    void Renderer::render() {

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

    MeshID Renderer::createMesh(const MeshDesc &meshDesc) {
        return 0;
    }

    void Renderer::transformMesh(MeshID mesh, const glm::mat4 &transform) {

    }

    void Renderer::removeMesh(MeshID mesh) {

    }
}
