#include "LayerStack.hpp"
#include <algorithm>

namespace Syrius {

    void LayerStack::pushLayer(SP<ILayer> layer) {
        m_Layers.push_back(layer);
        layer->onAttach();
    }

    void LayerStack::popLayer(LayerID layerID) {
        auto it = std::remove_if(m_Layers.begin(), m_Layers.end(),
        [layerID](const SP<ILayer>& layer) {
            if (layer->getID() == layerID) {
                layer->onDetach();  // call detach before removing
                return true;      // mark for removal
            }
            return false;
        });

        m_Layers.erase(it, m_Layers.end());
    }

    void LayerStack::onUpdate(const Duration deltaTime) const {
        for (const auto& layer : m_Layers){
            layer->onUpdate(deltaTime);
        }
    }

    void LayerStack::onEvent(const Event &event) {
        // reverse iterate through the layers
        for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it){
            if (!(*it)->onEvent(event)){
                break;
            }
        }
    }

    bool LayerStack::hasLayer(LayerID layerID) const {
        return std::any_of(m_Layers.begin(), m_Layers.end(),
                           [layerID](const SP<ILayer>& layer) { return layer->getID() == layerID; });
    }
}
