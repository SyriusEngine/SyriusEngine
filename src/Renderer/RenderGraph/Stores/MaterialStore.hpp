#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>
#include <SyriusUtils/Srstl/KeyVector.hpp>

#include "../IRenderGraphData.hpp"
#include "../Handles/MaterialHandle.hpp"

namespace Syrius::Renderer {

    class MaterialStore: public IRenderGraphData {
    public:
        MaterialStore(const ResourceView<Context>& ctx, RenderGraphContainer* container, const SP<DispatcherManager>& dispatcherManager);

        ~MaterialStore() override = default;

        void createMaterial(MaterialID materialID, const SP<Material>& material);

        void destroyMaterial(MaterialID materialID);

        void bindMeshMaterial(MeshID meshID, u32 startSlot);

    private:

        void createDefaultMaterial();

    private:
        Srstl::KeyVector<MaterialID, MaterialHandle> m_Materials;
        std::unordered_map<MeshID, MaterialID> m_MeshToMaterial;
    };
}