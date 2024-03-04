#include "SyriusEngineImpl.hpp"

namespace Syrius{

    SyriusEngineImpl::SyriusEngineImpl(const std::string &configFile) :
    SyriusEngine(configFile),
    m_LayerStack(),
    m_Config(configFile){
        setupWindow();
        setupRenderer();
    }

    SyriusEngineImpl::~SyriusEngineImpl() {
        m_Config["Window"]["Width"] = m_Window->getWidth();
        m_Config["Window"]["Height"] = m_Window->getHeight();
        m_Config["Window"]["XPos"] = m_Window->getPosX();
        m_Config["Window"]["YPos"] = m_Window->getPosY();
        m_Config.save();

    }

    void SyriusEngineImpl::run() {
        SR_PRECONDITION(m_Window.get() != nullptr, "Window is null (%p)", m_Window.get());

        while (m_Window->isOpen()){
            m_Renderer->render();
            m_Window->pollEvents();
            while (m_Window->hasEvent()){
                auto event = m_Window->getEvent();
                if (event.type == SR_EVENT_WINDOW_CLOSED){
                    m_Window->close();
                }
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
        wDesc.width = m_Config["Window"]["Width"].getOrDefault<uint32>(SR_DEFAULT_WIDTH);
        wDesc.height = m_Config["Window"]["Height"].getOrDefault<uint32>(SR_DEFAULT_HEIGHT);
        wDesc.xPos = m_Config["Window"]["XPos"].getOrDefault<int32>(200);
        wDesc.yPos = m_Config["Window"]["YPos"].getOrDefault<int32>(200);
        wDesc.title = m_Config["Window"]["Title"].getOrDefault<std::string>("SyriusEngine");
        m_Window = createWindow(wDesc);
    }

    void SyriusEngineImpl::setupRenderer() {
        RendererDesc rDesc;
        rDesc.graphicsAPI = static_cast<SR_SUPPORTED_API>(m_Config["Context"]["API"].getOrDefault<int>(SR_API_OPENGL));
        rDesc.vSync = m_Config["Context"]["VSync"].getOrDefault<bool>(true);
        rDesc.clearColor[0] = m_Config["Context"]["ClearR"].getOrDefault<float>(0.0f);
        rDesc.clearColor[1] = m_Config["Context"]["ClearG"].getOrDefault<float>(0.0f);
        rDesc.clearColor[2] = m_Config["Context"]["ClearB"].getOrDefault<float>(0.0f);
        rDesc.shaderLibraryPath = m_Config["Context"]["ShaderLibrary"].getOrDefault<std::string>("./Resources/Shaders");
        rDesc.enableExperimentalSRSLShaderCompiler = m_Config["Context"]["EnableExperimentalSRSLShaderCompiler"].getOrDefault<bool>(true);

        m_Renderer = createResource<Renderer>(rDesc, m_LayerStack, m_Window);
        m_RenderCommand = createResourceView<RenderCommand>(m_Renderer.get());
    }

    void SyriusEngineImpl::shutdown() {
        m_Window->close();
    }
}
