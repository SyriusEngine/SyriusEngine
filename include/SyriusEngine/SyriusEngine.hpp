#pragma once

#include <SyriusEngine/Renderer/RenderLayer.hpp>
#include <SyriusEngine/EngineConfiguration.hpp>
#include <SyriusEngine/Layer.hpp>

#include <SyriusEngine/Utils/EngineInclude.hpp>
#include <SyriusEngine/Utils/ExportEngine.hpp>

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>

namespace Syrius{

    class SR_ENGINE_API SyriusEngine {
    public:
        explicit SyriusEngine(const EngineConfiguration& config);

        ~SyriusEngine();

        void run() const;

        void pushLayer(const SP<ILayer> &layer) const;

        void popLayer(LayerID layerID) const;

        void pushRenderLayer(const SP<IRenderLayer>& renderLayer);

        void popRenderLayer(RenderLayerID layerID);

        MeshID createMesh(const Mesh& mesh);

        InstanceID createInstance(MeshID meshID);

        void destroyMesh(MeshID meshID);

        void destroyInstance(InstanceID instance);

        const UP<SyriusWindow>& getWindow() const { return m_Window; }

    private:

        class EngineData; // PIMPL

    private:
        UP<SyriusWindow> m_Window = nullptr;
        UP<EngineData> m_Data;
    };

}