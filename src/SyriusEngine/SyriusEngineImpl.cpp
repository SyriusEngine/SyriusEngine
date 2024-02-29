#include "SyriusEngineImpl.hpp"

namespace Syrius{

    SyriusEngineImpl::SyriusEngineImpl(const std::string &configFile) :
    SyriusEngine(configFile),
    m_LayerStack(),
    m_Config(configFile){
        WindowDesc wDesc;
        wDesc.width = m_Config["Window"]["Width"].getOrDefault<uint32>(SR_DEFAULT_WIDTH);
        wDesc.height = m_Config["Window"]["Height"].getOrDefault<uint32>(SR_DEFAULT_HEIGHT);
        wDesc.xPos = m_Config["Window"]["XPos"].getOrDefault<int32>(200);
        wDesc.yPos = m_Config["Window"]["YPos"].getOrDefault<int32>(200);
        wDesc.title = m_Config["Window"]["Title"].getOrDefault<std::string>("SyriusEngine");
        m_Window = createWindow(wDesc);
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

            m_Window->pollEvents();
            while (m_Window->hasEvent()){
                auto event = m_Window->getEvent();
                if (event.type == SR_EVENT_WINDOW_CLOSED){
                    m_Window->close();
                }
                m_LayerStack.onEvent(event);
            }

            m_LayerStack.onUpdate();


        }
    }

    void SyriusEngineImpl::pushLayer(Resource<Layer> layer) {
        m_LayerStack.pushLayer(std::move(layer));
    }

    void SyriusEngineImpl::pushRenderLayer(Resource<RenderLayer> layer) {

    }
}
