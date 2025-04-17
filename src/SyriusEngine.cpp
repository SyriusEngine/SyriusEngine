#include <SyriusEngine/SyriusEngine.hpp>
#include "EngineData.hpp"

namespace Syrius{

    SyriusEngine::SyriusEngine(const EngineConfiguration& config):
    m_Data(createUP<EngineData>()) {
        WindowDesc windowDesc;
        windowDesc.width = config.windowWidth;
        windowDesc.height = config.windowHeight;
        windowDesc.title = "SyriusEngine";
        m_Window = createWindow(windowDesc);

        RendererDesc rendererDesc;
        rendererDesc.api = config.api;
        rendererDesc.enableVsync = config.vsync;

        m_Data->renderer = createUP<Renderer>(m_Window, m_Data->dispatcherManager, rendererDesc);
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
            Duration deltaTime = currentTime - lastFrameTime;
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
    }

    void SyriusEngine::popLayer(const LayerID layerID) const {
        m_Data->layerStack.popLayer(layerID);
    }

    void SyriusEngine::pushRenderLayer(const SP<IRenderLayer> &renderLayer) {
        m_Data->renderer->pushRenderLayer(renderLayer);
    }

    void SyriusEngine::popRenderLayer(RenderLayerID layerID) {
        m_Data->renderer->popRenderLayer(layerID);
    }

    MeshID SyriusEngine::createMesh(const Mesh &mesh) {
        MeshID meshID = generateID();
        // TODO: Dispatch mesh to the renderer
        return meshID;
    }

    void SyriusEngine::updateMesh(MeshID meshID, const Mesh &mesh) {

    }

    void SyriusEngine::destroyMesh(MeshID meshID) {

    }


}
