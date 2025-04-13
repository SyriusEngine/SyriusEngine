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
    }

    SyriusEngine::~SyriusEngine() {

    }

    void SyriusEngine::run() const {
        SR_PRECONDITION(m_Window != nullptr, "Window is null");

        while (m_Window->isOpen()) {
            // 1. Events
            m_Window->pollEvents();
            while (m_Window->hasEvent()) {
                auto event = m_Window->getNextEvent();
                m_Data->layerStack.onEvent(event);
            }

            // 2. Update
            m_Data->layerStack.onUpdate();
        }
    }

    void SyriusEngine::pushLayer(const SP<ILayer> &layer) const {
        m_Data->layerStack.pushLayer(layer);
    }

    void SyriusEngine::popLayer(const LayerID layerID) const {
        m_Data->layerStack.popLayer(layerID);
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
