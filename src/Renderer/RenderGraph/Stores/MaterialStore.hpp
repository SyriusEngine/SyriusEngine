#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>
#include <SyriusUtils/Srstl/KeyVector.hpp>

#include "../Handles/MaterialHandle.hpp"

namespace Syrius::Renderer {

    class MaterialStore {
    public:
        explicit MaterialStore(const ResourceView<Context>& ctx);

        ~MaterialStore() = default;

        void createMaterial(MaterialID materialID, const Material& material, const ResourceView<Context>& ctx);

        void destroyMaterial(MaterialID materialID, const ResourceView<Context>& ctx);

        void bindMaterial(MaterialID materialID, u32 startSlot);

    private:
        Srstl::KeyVector<MaterialID, MaterialHandle> m_Materials;
    };
}