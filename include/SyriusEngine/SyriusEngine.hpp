#pragma once

#include <SyriusEngine/Renderer/RenderLayer.hpp>
#include <SyriusEngine/EngineConfiguration.hpp>
#include <SyriusEngine/Layer.hpp>

#include <SyriusEngine/Utils/EngineInclude.hpp>
#include <SyriusEngine/Utils/ExportEngine.hpp>

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>
#include <SyriusEngine/Renderer/BuiltinMeshes.hpp>

namespace Syrius{

    class SR_ENGINE_API SyriusEngine {
    public:
        explicit SyriusEngine(const EngineConfiguration& config);

        ~SyriusEngine();

        void run() const;

        void pushLayer(const SP<ILayer> &layer) const;

        void popLayer(LayerID layerID) const;

        void pushRenderLayer(const SP<IRenderLayer>& renderLayer) const;

        void popRenderLayer(RenderLayerID layerID) const;

        MeshID createMesh(const Mesh& mesh) const;

        InstanceID createInstance(MeshID meshID) const;

        void destroyMesh(MeshID meshID) const;

        void destroyInstance(InstanceID instance) const;

        void setInstanceTransform(InstanceID instanceID, const Transform& transform) const;

        void setProjection(ProjectionID projectionID, const Projection& projection) const;

        void setCamera(CameraID cameraID, const Camera& camera) const;

        MaterialID createMaterial(SP<Material> material) const;

        void meshSetMaterial(MeshID meshID, MaterialID materialID) const;

        void destroyMaterial(MaterialID materialID) const;

        LightID createLight(const Light& light) const;

        void setLight(LightID lightID, const Light& light) const;

        void destroyLight(LightID lightID) const;

        const UP<SyriusWindow>& getWindow() const { return m_Window; }

    private:

        void setupWindow(const EngineConfiguration& config);

        void setupRenderer(const EngineConfiguration& config);

        class EngineData; // PIMPL

    private:
        UP<SyriusWindow> m_Window = nullptr;
        UP<EngineData> m_Data;
    };

}