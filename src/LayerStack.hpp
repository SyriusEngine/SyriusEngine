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

    private:
        std::vector<SP<ILayer>> m_Layers;
    };

}