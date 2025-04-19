#include <SyriusEngine/SyriusEngine.hpp>
#include "EngineData.hpp"
#include <SyriusUtils/Clock/ClockMacros.hpp>

namespace Syrius{

    SyriusEngine::SyriusEngine(const EngineConfiguration& config):
    m_Data(createUP<EngineData>()) {
        SR_START_TIMER("EngineStartup");

        WindowDesc windowDesc;
        windowDesc.width = config.windowWidth;
        windowDesc.height = config.windowHeight;
        windowDesc.title = "SyriusEngine";
        m_Window = createWindow(windowDesc);

        Renderer::RendererDesc rendererDesc;
        rendererDesc.api = config.api;
        rendererDesc.enableVsync = config.vsync;
        rendererDesc.rendererSystem = SR_RENDERER_SYSTEM_DEFAULT;
        rendererDesc.shaderDirectory = config.shaderDirectory;

        m_Data->renderer = createUP<Renderer::Renderer>(m_Window, m_Data->dispatcherManager, rendererDesc);

        SR_STOP_TIMER("EngineStartup");
        const Duration startupTime = SR_GET_TIME("EngineStartup");
        SR_LOG_INFO("SyriusEngine", "Setup Completed (took {})!", startupTime)
    }

    SyriusEngine::~SyriusEngine() {

    }

    void SyriusEngine::run() const {
        SR_PRECONDITION(m_Window != nullptr, "Window is null");

        TimePoint lastFrameTime = getTime();
        while (m_Window->isOpen()) {
            // 1. Events
            m_Window->pollEvents();
            while (m_Window->hasEvent()) {
                auto event = m_Window->getNextEvent();
                m_Data->layerStack.onEvent(event);
            }

            // 2. Update
            auto currentTime = getTime();
            const Duration deltaTime = currentTime - lastFrameTime;
            lastFrameTime = currentTime;
            m_Data->layerStack.onUpdate(deltaTime);

            // 3. Render
            m_Data->renderer->render();

            // 4. Swap buffers
            m_Data->renderer->swapFrontAndBackBuffer();
        }
    }

    void SyriusEngine::pushLayer(const SP<ILayer> &layer) const {
        m_Data->layerStack.pushLayer(layer);
        SR_LOG_INFO("SyriusEngine", "Pushed Layer: {}", layer->getID());
    }

    void SyriusEngine::popLayer(const LayerID layerID) const {
        m_Data->layerStack.popLayer(layerID);
        SR_LOG_INFO("SyriusEngine", "Popped Layer: {}", layerID);
    }

    void SyriusEngine::pushRenderLayer(const SP<IRenderLayer> &renderLayer) {
        m_Data->renderer->pushRenderLayer(renderLayer);
        SR_LOG_INFO("SyriusEngine", "Pushed Render Layer: {}", renderLayer->getID());
    }

    void SyriusEngine::popRenderLayer(RenderLayerID layerID) {
        m_Data->renderer->popRenderLayer(layerID);
        SR_LOG_INFO("SyriusEngine", "Popped Render Layer: {}", layerID);
    }

    MeshID SyriusEngine::createMesh(const Mesh &mesh) {
        return m_Data->dispatchDataCreate<MeshID, Mesh>(mesh);
    }

    InstanceID SyriusEngine::createInstance(MeshID meshID) {
        return m_Data->dispatchDataCreate<InstanceID, MeshID>(meshID);
    }

    void SyriusEngine::destroyMesh(MeshID meshID) {


    }


}
