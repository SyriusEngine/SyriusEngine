#pragma once

#include "Layer.hpp"

namespace Syrius{

    class SR_API RenderLayer : public Layer{
    public:
        RenderLayer() = default;

        ~RenderLayer() override = default;

        virtual void onRendererAttach(ResourceView<Context>& context) = 0;

        virtual void onRendererDetach(ResourceView<Context>& context) = 0;

        virtual void onRender(ResourceView<Context>& context) = 0;
    };
}