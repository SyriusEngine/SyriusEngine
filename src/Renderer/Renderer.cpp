#include "Renderer.hpp"

namespace Syrius {

     Renderer::Renderer(UP<SyriusWindow> &window, SP<DispatcherManager> dispatcherManager, const RendererDesc &desc):
     m_Window(window),
     m_DispatcherManager(dispatcherManager),
     m_Worker("RenderThread") {
          bool renderThreadSetupFinished = false;
          m_Worker.add([this, &renderThreadSetupFinished, desc] {
               setupContext(desc);
               renderThreadSetupFinished = true;
          });
          // While the render thread does its setup, the main thread can add the
          // communication (dispatcher) logic
          setupDispatchers();
          while (!renderThreadSetupFinished) {
               std::this_thread::sleep_for(1.0ms);
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
          m_RenderLayers.push_back(renderLayer);
     }

     void Renderer::popRenderLayer(RenderLayerID layerID) {
          auto it = std::remove_if(m_RenderLayers.begin(), m_RenderLayers.end(),
               [layerID](const SP<IRenderLayer>& layer) {
                   if (layer->getID() == layerID) {
                       return true;      // mark for removal
                   }
                   return false;
               });
          m_RenderLayers.erase(it, m_RenderLayers.end());
     }

     void Renderer::render() {  }

     void Renderer::swapFrontAndBackBuffer() {
          m_Worker.addSync([this] {
               m_Context->swapBuffers();
          });
     }

     void Renderer::setupDispatchers() {  }

     void Renderer::setupContext(const RendererDesc &desc) {
          ContextDesc contextDesc;
          contextDesc.api = desc.api;
          m_Context = m_Window->createContext(contextDesc);

          m_Context->setVerticalSynchronisation(desc.enableVsync);
     }

     void Renderer::terminateContext() {
          m_Window->destroyContext();
     }
}
