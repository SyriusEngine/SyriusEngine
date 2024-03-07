#include "LayerStack.hpp"

namespace Syrius{

    LayerStack::LayerStack():
    m_Layers(){

    }

    LayerStack::~LayerStack() {

    }

    void LayerStack::pushLayer(Resource<Layer> layer) {
        SR_PRECONDITION(layer != nullptr, "Layer (%p) cannot be null", layer.get());

        layer->onAttach();
        m_Layers.push_back(std::move(layer));
    }

    void LayerStack::pushRenderLayer(ResourceView<RenderLayer> layer, ResourceView<Context> &context) {
        SR_PRECONDITION(layer.get() != nullptr, "RenderLayer (%p) cannot be null", layer.get());
        SR_PRECONDITION(std::find_if(m_Layers.begin(), m_Layers.end(), [layer](const Resource<Layer>& l){
            return l.get() == layer.get();
        }) != m_Layers.end(), "RenderLayer (%p) is not on the stack", layer.get());

        layer->onRendererAttach(context);
        m_RenderLayers.push_back(layer);
    }


    void LayerStack::onUpdate() {
        for (auto& layer : m_Layers){
            layer->onUpdate();
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

    void LayerStack::onRender(ResourceView<Context> &context) {
        for (auto& layer : m_RenderLayers){
            layer->onRender(context);
        }
    }

    void LayerStack::detachRenderLayers(ResourceView<Context> &context) {
        for (auto& layer : m_RenderLayers){
            layer->onRendererDetach(context);
        }
    }

    void LayerStack::clearLayers() {
        for (auto& layer : m_Layers){
            layer->onDetach();
        }
        m_Layers.clear();
        m_RenderLayers.clear();
    }

}
