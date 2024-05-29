#include "SyriusEngineImpl.hpp"

namespace Syrius{

    SyriusEngineImpl::SyriusEngineImpl(const EngineConfiguration& config) :
    SyriusEngine(config),
    m_LayerStack(){
        setupWindow();
        setupRenderer();
    }

    SyriusEngineImpl::~SyriusEngineImpl() {
        m_LayerStack.clearLayers(); // destroy all layers before the renderer is destroyed

        m_Config.windowWidth = m_Window->getWidth();
        m_Config.windowHeight = m_Window->getHeight();
        m_Config.windowPosX = m_Window->getPosX();
        m_Config.windowPosY = m_Window->getPosY();
        m_Config.save();
    }

    void SyriusEngineImpl::run() {
        SR_PRECONDITION(m_Window.get() != nullptr, "Window is null (%p)", m_Window.get());

        while (m_Window->isOpen()){
            m_Renderer->render();
            m_Window->pollEvents();
            while (m_Window->hasEvent()){
                auto event = m_Window->getEvent();
                m_LayerStack.onEvent(event);
            }

            m_LayerStack.onUpdate();

            m_Renderer->swapBuffers();
        }
    }

    void SyriusEngineImpl::pushLayer(Resource<Layer> layer) {
        m_LayerStack.pushLayer(std::move(layer));
    }

    void SyriusEngineImpl::pushRenderLayer(Resource<RenderLayer> layer) {
        m_Renderer->pushRenderLayer(std::move(layer));
    }

    void SyriusEngineImpl::setupWindow() {
        WindowDesc wDesc;
        wDesc.width = m_Config.windowWidth;
        wDesc.height = m_Config.windowHeight;
        wDesc.xPos = m_Config.windowPosX;
        wDesc.yPos = m_Config.windowPosY;
        wDesc.title = m_Config.windowTitle;
        m_Window = createWindow(wDesc);
    }

    void SyriusEngineImpl::setupRenderer() {
        RendererDesc rDesc;
        rDesc.graphicsAPI = m_Config.graphicsAPI;
        rDesc.vSync = m_Config.enableVSync;
        rDesc.clearColor[0] = m_Config.clearColor[0];
        rDesc.clearColor[1] = m_Config.clearColor[1];
        rDesc.clearColor[2] = m_Config.clearColor[2];
        rDesc.shaderLibraryPath = m_Config.shaderLibraryPath;
        rDesc.enableExperimentalSRSLShaderCompiler = m_Config.enableSrslShaders;

        m_Renderer = createResource<Renderer>(rDesc, m_LayerStack, m_Window);
        m_RenderCommand = ResourceView<RenderCommand>(m_Renderer);
    }

    void SyriusEngineImpl::shutdown() {
        m_Window->close();
    }
}
