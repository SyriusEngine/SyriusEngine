#pragma once

#include <SyriusEngine/Layer.hpp>

namespace Syrius {

    class LayerStack {
    public:
        LayerStack() = default;

        ~LayerStack() = default;

        void pushLayer(SP<ILayer> layer);

        void popLayer(LayerID layerID);

        void onUpdate(Duration deltaTime) const;

        void onEvent(const Event& event);

        [[nodiscard]] Size getLayerCount() const {
            return m_Layers.size();
        }

        bool hasLayer(LayerID layerID) const;

    private:
        std::vector<SP<ILayer>> m_Layers;
    };

}