#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>
#include <SyriusEngine/Renderer/RenderLayer.hpp>

#include "../Communication/DispatcherManager.hpp"
#include "../Threading/Worker.hpp"

namespace Syrius::Renderer {

    struct RendererDesc {
        SR_SUPPORTED_API api = SR_API_OPENGL;
        SR_RENDERER_SYSTEM rendererSystem = SR_RENDERER_SYSTEM_DEFAULT;
        bool enableVsync = false;
        fs::path shaderDirectory = fs::current_path();
    };

    class Renderer {
    public:
        Renderer() = delete;

        Renderer(UP<SyriusWindow>& window, const SP<DispatcherManager> &dispatcherManager, const SP<WorkerPool>& workerPool, const RendererDesc& desc);

        ~Renderer();

        void pushRenderLayer(SP<IRenderLayer> renderLayer);

        void popRenderLayer(RenderLayerID layerID);

        void render();

        void swapFrontAndBackBuffer();

    private:

        void setupContext(const RendererDesc& desc);

        void terminateContext();

    private:
        UP<SyriusWindow>& m_Window;
        SP<DispatcherManager> m_DispatcherManager;
        SP<WorkerPool> m_WorkerPool;
        std::vector<SP<IRenderLayer>> m_RenderLayers;
        ResourceView<Context> m_Context;

    };
}
