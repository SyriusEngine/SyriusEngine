#include "Renderer.hpp"

#include "RenderGraph/RenderGraphLayer.hpp"

namespace Syrius::Renderer {

    /*
     * With all lambda expressions here, we do not want to take in these parameters by reference, we want to make a
     * copy of them. This is because the lambda expressions are executed in a different thread and we do not want
     * to have dangling references. But copying is expensive, so every data that is passed to the lambda functions
     * should be wrapped in a shared pointer. A Copy will increase the reference count of the shared pointer,
     * preventing the object from being destroyed.
     */

    Renderer::Renderer(UP<SyriusWindow> &window, const SP<DispatcherManager> &dispatcherManager, const SP<WorkerPool>& workerPool, const RendererDesc &desc) :
        m_Window(window), m_DispatcherManager(dispatcherManager), m_WorkerPool(workerPool) {
        m_WorkerPool->addTaskSync(SR_WORKER_RENDERER, [&] {
            setupContext(desc);
            const auto renderGraphLayer = createSP<RenderGraphLayer>(desc.shaderDirectory, m_DispatcherManager);
            m_RenderLayers.push_back(renderGraphLayer);
            renderGraphLayer->onRendererAttach(m_Context);
        });
    }

    Renderer::~Renderer() {
        m_WorkerPool->addTaskSync(SR_WORKER_RENDERER, [&] {
            terminateContext();
        });
    }

    void Renderer::pushRenderLayer(SP<IRenderLayer> renderLayer) {
        m_WorkerPool->addTask(SR_WORKER_RENDERER, [this, renderLayer] {
            m_RenderLayers.push_back(renderLayer);
            renderLayer->onRendererAttach(m_Context);
        });
    }

    void Renderer::popRenderLayer(RenderLayerID layerID) {
        m_WorkerPool->addTask(SR_WORKER_RENDERER, [this, layerID] {
            const auto it =
            std::remove_if(m_RenderLayers.begin(), m_RenderLayers.end(), [layerID](const SP<IRenderLayer> &layer) {
                if (layer->getID() == layerID) {
                    return true; // mark for removal
                }
                return false;
            });
            (*it)->onRendererDetach(m_Context);
            m_RenderLayers.erase(it, m_RenderLayers.end());
        });
    }

    void Renderer::render() {
        m_WorkerPool->addTask(SR_WORKER_RENDERER, [this] {
            m_Context->clear();
            for (const auto &layer: m_RenderLayers) {
                layer->onRender(m_Context);
            }
        });
    }

    void Renderer::swapFrontAndBackBuffer() {
        m_WorkerPool->addTaskSync(SR_WORKER_RENDERER, [this] { m_Context->swapBuffers(); });
    }

    void Renderer::setupContext(const RendererDesc &desc) {
        ContextDesc contextDesc;
        contextDesc.api = desc.api;
        m_Context = m_Window->createContext(contextDesc);

        m_Context->setVerticalSynchronisation(desc.enableVsync);
        SR_LOG_INFO("Renderer", "VSync: {}", desc.enableVsync);
    }

    void Renderer::terminateContext() { m_Window->destroyContext(); }

} // namespace Syrius
