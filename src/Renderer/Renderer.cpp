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

    Renderer::Renderer(UP<SyriusWindow> &window, SP<DispatcherManager> dispatcherManager, const RendererDesc &desc) :
        m_Window(window), m_DispatcherManager(dispatcherManager), m_Worker("RenderThread") {
        bool renderThreadSetupFinished = false;
        m_Worker.add([&] {
            setupContext(desc);
            renderThreadSetupFinished = true;
        });
        // While the render thread does its setup, the main thread can add the
        // communication (dispatcher) logic
        setupDispatchers();
        while (!renderThreadSetupFinished) {
            std::this_thread::sleep_for(1.0ms);
        }

        // Set the initial render layer
        switch (desc.rendererSystem) {
            case SR_RENDERER_SYSTEM_DEFAULT: {
                auto renderGraphLayer = createSP<RenderGraphLayer>(desc.shaderDirectory);
                pushRenderLayer(renderGraphLayer);
                break;
            }
            default: break;
        }
    }

    Renderer::~Renderer() {
        bool isTerminated = false;
        m_Worker.add([&] {
            terminateContext();
            isTerminated = true;
        });
        while (!isTerminated) {
            std::this_thread::sleep_for(1.0ms);
        }
        m_Worker.stop();
    }

    void Renderer::pushRenderLayer(SP<IRenderLayer> renderLayer) {
        m_Worker.add([this, renderLayer] {
            m_RenderLayers.push_back(renderLayer);
            renderLayer->onRendererAttach(m_Context);
        });
    }

    void Renderer::popRenderLayer(RenderLayerID layerID) {
        m_Worker.add([this, layerID] {
            auto it =
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
        m_Worker.add([this] {
            m_Context->clear();
            for (const auto &layer: m_RenderLayers) {
                layer->onRender(m_Context);
            }
        });
    }

    void Renderer::swapFrontAndBackBuffer() {
        m_Worker.addSync([this] { m_Context->swapBuffers(); });
    }

    void Renderer::setupDispatchers() {
        const auto meshDispatcher = m_DispatcherManager->getDispatcher<MeshID, Mesh>();
        meshDispatcher->registerCreate([this](const MeshID meshID, const SP<Mesh> &mesh) { createMesh(meshID, mesh); });
        meshDispatcher->registerDelete([&](const MeshID meshID) { destroyMesh(meshID); });

        const auto instanceDispatcher = m_DispatcherManager->getDispatcher<InstanceID, MeshID>();
        instanceDispatcher->registerCreate([this](const InstanceID instanceID, const SP<MeshID> &meshID) {
            createInstance(instanceID, meshID);
        });
        instanceDispatcher->registerDelete([&](const InstanceID instanceID) { destroyInstance(instanceID); });
    }

    void Renderer::setupContext(const RendererDesc &desc) {
        ContextDesc contextDesc;
        contextDesc.api = desc.api;
        m_Context = m_Window->createContext(contextDesc);

        m_Context->setVerticalSynchronisation(desc.enableVsync);
        SR_LOG_INFO("Renderer", "VSync: {}", desc.enableVsync);
    }

    void Renderer::terminateContext() { m_Window->destroyContext(); }

    void Renderer::createMesh(const MeshID meshID, SP<Mesh> mesh) {
        m_Worker.add([this, meshID, mesh] {
            for (const auto &layer: m_RenderLayers) {
                layer->createMesh(meshID, *mesh, m_Context);
            }
        });
    }

    void Renderer::createInstance(const InstanceID instanceID, const SP<MeshID>& meshID) {
        m_Worker.add([this, instanceID, meshID] {
            for (const auto &layer: m_RenderLayers) {
                layer->createInstance(instanceID, *meshID, m_Context);
            }
        });
    }

    void Renderer::destroyMesh(const MeshID meshID) {
        m_Worker.add([this, meshID] {
            for (const auto &layer: m_RenderLayers) {
                layer->destroyMesh(meshID, m_Context);
            }
        });
    }

    void Renderer::destroyInstance(const InstanceID instanceID) {
        m_Worker.add([this, instanceID] {
            for (const auto &layer: m_RenderLayers) {
                layer->destroyInstance(instanceID);
            }
        });
    }

} // namespace Syrius
