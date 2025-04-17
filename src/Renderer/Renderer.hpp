#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>
#include <SyriusEngine/Renderer/RenderLayer.hpp>

#include "../Communication/DispatcherManager.hpp"
#include "../Utils/Worker.hpp"

namespace Syrius {

    struct RendererDesc {
        SR_SUPPORTED_API api = SR_API_OPENGL;
        bool enableVsync = false;
    };

    class Renderer {
    public:
        Renderer() = delete;

        Renderer(UP<SyriusWindow>& window, SP<DispatcherManager> dispatcherManager, const RendererDesc& desc);

        ~Renderer();

        void pushRenderLayer(SP<IRenderLayer> renderLayer);

        void popRenderLayer(RenderLayerID layerID);

        void render();

        void swapFrontAndBackBuffer();

    private:

        void setupDispatchers();

        void setupContext(const RendererDesc& desc);

        void terminateContext();

    private:
        UP<SyriusWindow>& m_Window;
        SP<DispatcherManager> m_DispatcherManager;
        std::vector<SP<IRenderLayer>> m_RenderLayers;
        ResourceView<Context> m_Context;
        Worker m_Worker;

    };
}
