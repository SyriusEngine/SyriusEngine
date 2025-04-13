#pragma once

#include <SyriusEngine/EngineConfiguration.hpp>
#include <SyriusEngine/Layer.hpp>

#include <SyriusEngine/Utils/ExportEngine.hpp>
#include <SyriusEngine/Utils/EngineInclude.hpp>

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>

namespace Syrius{

    class SR_ENGINE_API SyriusEngine {
    public:
        explicit SyriusEngine(const EngineConfiguration& config);

        ~SyriusEngine();

        void run() const;

        void pushLayer(const SP<ILayer> &layer) const;

        void popLayer(LayerID layerID) const;

        MeshID createMesh(const Mesh& mesh);

        void updateMesh(MeshID meshID, const Mesh& mesh);

        void destroyMesh(MeshID meshID);

    private:

        class EngineData; // PIMPL

    private:
        UP<SyriusWindow> m_Window = nullptr;
        UP<EngineData> m_Data = nullptr;
    };

}