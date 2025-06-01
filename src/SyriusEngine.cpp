#include <SyriusEngine/SyriusEngine.hpp>
#include "EngineData.hpp"
#include <SyriusUtils/Clock/ClockMacros.hpp>

static void debugMessageHandler(const Syrius::Message& msg) {
    std::cout << "[" << msg.source << "] [" << Syrius::getMessageSeverityString(msg.severity) << "]: " << msg.message << std::endl;
}

namespace Syrius{

    SyriusEngine::SyriusEngine(const EngineConfiguration& config):
    m_Data(createUP<EngineData>()) {
        SR_START_TIMER("EngineStartup");
        Logger::setDebugCallback(debugMessageHandler);


        setupWindow(config);
        setupRenderer(config);

        SR_STOP_TIMER("EngineStartup");
        const Duration startupTime = SR_GET_TIME("EngineStartup");
        SR_LOG_INFO("SyriusEngine", "Setup Completed (took {})!", startupTime)
    }

    SyriusEngine::~SyriusEngine() {

    }

    void SyriusEngine::run() const {
        SR_PRECONDITION(m_Window != nullptr, "Window is null");

        TimePoint lastFrameTime = getTime();
        while (m_Window->isOpen()) {
            // 1. Events
            m_Window->pollEvents();
            while (m_Window->hasEvent()) {
                auto event = m_Window->getNextEvent();
                m_Data->layerStack.onEvent(event);
            }

            // 2. Update
            auto currentTime = getTime();
            const Duration deltaTime = currentTime - lastFrameTime;
            lastFrameTime = currentTime;
            m_Data->layerStack.onUpdate(deltaTime);

            // 3. Render
            m_Data->renderer->render();

            // 4. Swap buffers
            m_Data->renderer->swapFrontAndBackBuffer();
        }
    }

    void SyriusEngine::pushLayer(const SP<ILayer> &layer) const {
        m_Data->layerStack.pushLayer(layer);
        SR_LOG_INFO("SyriusEngine", "Pushed Layer: {}", layer->getID());
    }

    void SyriusEngine::popLayer(const LayerID layerID) const {
        m_Data->layerStack.popLayer(layerID);
        SR_LOG_INFO("SyriusEngine", "Popped Layer: {}", layerID);
    }

    void SyriusEngine::pushRenderLayer(const SP<IRenderLayer> &renderLayer) const {
        m_Data->renderer->pushRenderLayer(renderLayer);
        SR_LOG_INFO("SyriusEngine", "Pushed Render Layer: {}", renderLayer->getID());
    }

    void SyriusEngine::popRenderLayer(RenderLayerID layerID) const {
        m_Data->renderer->popRenderLayer(layerID);
        SR_LOG_INFO("SyriusEngine", "Popped Render Layer: {}", layerID);
    }

    MeshID SyriusEngine::createMesh(const Mesh &mesh) const {
        return m_Data->dispatchDataCreate<MeshID, Mesh>(mesh);
    }

    InstanceID SyriusEngine::createInstance(MeshID meshID) const {
        return m_Data->dispatchDataCreate<InstanceID, MeshID>(meshID);
    }

    void SyriusEngine::destroyMesh(const MeshID meshID) const {
        m_Data->dispatchDataDelete<MeshID, Mesh>(meshID);
    }

    void SyriusEngine::destroyInstance(const InstanceID instance) const {
        m_Data->dispatchDataDelete<InstanceID, MeshID>(instance);
    }

    void SyriusEngine::setInstanceTransform(const InstanceID instanceID, const Transform &transform) const {
        m_Data->dispatchDataUpdate<InstanceID, Transform>(instanceID, transform);
    }

    void SyriusEngine::setProjection(const ProjectionID projectionID, const Projection &projection) const {
        m_Data->dispatchDataUpdate<ProjectionID, Projection>(projectionID, projection);
    }

    void SyriusEngine::setCamera(const CameraID cameraID, const Camera &camera) const {
        m_Data->dispatchDataUpdate<CameraID, Camera>(cameraID, camera);
    }

    MaterialID SyriusEngine::createMaterial(const SP<Material>& material) const {
        MaterialID key = generateID();
        const auto dispatcher = m_Data->dispatcherManager->getDispatcher<MaterialID, Material>();
        dispatcher->dispatchCreate(key, material);
        return key;
    }

    void SyriusEngine::meshSetMaterial(const MeshID meshID, const MaterialID materialID) const {
        m_Data->dispatchDataUpdate<MeshID, MaterialID>(meshID, materialID);
    }

    void SyriusEngine::destroyMaterial(MaterialID materialID) const {
        m_Data->dispatchDataDelete<MaterialID, Material>(materialID);
    }

    LightID SyriusEngine::createLight(const Light &light) const {
        return m_Data->dispatchDataCreate<LightID, Light>(light);
    }

    void SyriusEngine::setLight(const LightID lightID, const Light &light) const {
        m_Data->dispatchDataUpdate<LightID, Light>(lightID, light);
    }

    void SyriusEngine::destroyLight(const LightID lightID) const {
        m_Data->dispatchDataDelete<LightID, Light>(lightID);
    }

    void SyriusEngine::setupWindow(const EngineConfiguration &config) {
        WindowDesc windowDesc;
        windowDesc.width = config.windowWidth;
        windowDesc.height = config.windowHeight;
        windowDesc.title = "SyriusEngine";
        m_Window = createWindow(windowDesc);
    }

    void SyriusEngine::setupRenderer(const EngineConfiguration &config) {
        SR_PRECONDITION(m_Window != nullptr, "Window is null");

        Renderer::RendererDesc rendererDesc;
        rendererDesc.api = config.api;
        rendererDesc.enableVsync = config.vsync;
        rendererDesc.rendererSystem = SR_RENDERER_SYSTEM_DEFAULT;
        rendererDesc.shaderDirectory = config.shaderDirectory;

        m_Data->renderer = createUP<Renderer::Renderer>(m_Window, m_Data->dispatcherManager, m_Data->workerPool, rendererDesc);

        // Setup default projection
        Projection defaultProjection;
        defaultProjection.windowWidth = static_cast<float>(m_Window->getWidth());
        defaultProjection.windowHeight = static_cast<float>(m_Window->getHeight());
        defaultProjection.fov = 45.0f;
        defaultProjection.nearPlane = 0.1f;
        defaultProjection.farPlane = 100.0f;
        m_Data->dispatchDataUpdate<ProjectionID, Projection>(SR_DEFAULT_PROJECTION, defaultProjection);
    }
}
