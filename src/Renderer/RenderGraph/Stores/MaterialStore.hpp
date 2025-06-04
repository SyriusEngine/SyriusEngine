#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>
#include <SyriusUtils/Srstl/KeyVector.hpp>
#include <SyriusUtils/DevUtils/TestingMacros.hpp>

#include "../Handles/MaterialHandle.hpp"
#include "../IRenderGraphData.hpp"

namespace Syrius::Renderer {

    class MaterialStore: public IRenderGraphData {
    public:
        MaterialStore(const ResourceView<Context>& ctx, RenderGraphContainer* container, const SP<DispatcherManager>& dispatcherManager);

        ~MaterialStore() override = default;

        void bindMeshMaterial(MeshID meshID, u32 startSlot);

    private:

        void createMaterial(MaterialID materialID, const SP<Material>& material);

        void destroyMaterial(MaterialID materialID);

        void createDefaultMaterial();

    private:
        Srstl::KeyVector<MaterialID, MaterialHandle> m_Materials;
        std::unordered_map<MeshID, MaterialID> m_MeshToMaterial;

        SR_GET_PRIVATE_MEMBER(MaterialStore,  m_Materials);
        SR_GET_PRIVATE_MEMBER(MaterialStore, m_MeshToMaterial);
    };
}