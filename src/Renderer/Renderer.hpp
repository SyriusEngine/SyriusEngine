#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>
#include <SyriusEngine/Renderer/RenderLayer.hpp>

#include "../Communication/DispatcherManager.hpp"
#include "../Utils/Worker.hpp"

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

        Renderer(UP<SyriusWindow>& window, const SP<DispatcherManager> &dispatcherManager, const RendererDesc& desc);

        ~Renderer();

        void pushRenderLayer(SP<IRenderLayer> renderLayer);

        void popRenderLayer(RenderLayerID layerID);

        void render();

        void swapFrontAndBackBuffer();

    private:

        void setupDispatchers();

        void setupContext(const RendererDesc& desc);

        void terminateContext();

        void createMesh(MeshID meshID, const SP<Mesh>& mesh);

        void createInstance(InstanceID instanceID, const SP<MeshID>& meshID);

        void destroyMesh(MeshID meshID);

        void destroyInstance(InstanceID instanceID);

        void setInstanceTransform(InstanceID instanceID, const SP<Transform>& transform);

        void setProjection(ProjectionID projectionID, const SP<Projection>& projection);

        void setCamera(CameraID cameraID, const SP<Camera>& camera);

        void createMaterial(MaterialID materialID, const SP<Material>& material);

        void setMeshMaterial(MeshID meshID, const SP<MaterialID>& materialID);

        void destroyMaterial(MaterialID materialID);

    private:
        UP<SyriusWindow>& m_Window;
        SP<DispatcherManager> m_DispatcherManager;
        std::vector<SP<IRenderLayer>> m_RenderLayers;
        ResourceView<Context> m_Context;
        Worker m_Worker;

    };
}
