#pragma once

#include "Core/InternalInclude.hpp"
#include "../../include/SyriusEngine/Layer.hpp"
#include "../../include/SyriusEngine/RenderLayer.hpp"

namespace Syrius{

    class LayerStack{
    public:
        LayerStack();

        ~LayerStack();

        void pushLayer(Resource<Layer> layer);

        /**
         * @brief Pushes a render layer onto the stack. The layer object should already be on the stack.
         * @param layer The render layer to push
         * @param context The context to attach the layer to
         */
        void pushRenderLayer(ResourceView<RenderLayer> layer, ResourceView<Context>& context);

        void onUpdate();

        void onEvent(const Event& event);

        void onRender(ResourceView<Context>& context);

        void detachRenderLayers(ResourceView<Context>& context);

        void clearLayers();


    private:
        std::vector<Resource<Layer>> m_Layers;
        std::vector<ResourceView<RenderLayer>> m_RenderLayers;
    };

}