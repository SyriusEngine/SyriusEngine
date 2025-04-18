#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>

#include "../Handles/MeshHandle.hpp"
#include "ShaderStore.hpp"

namespace Syrius::Renderer {

    class GeometryStore {
    public:
        explicit GeometryStore(UP<ShaderStore>& shaderStore, const ResourceView<Context>& ctx);

        ~GeometryStore() = default;

        void createMesh(MeshID meshID, const Mesh& mesh, const ResourceView<Context>& ctx);

        void createInstance(InstanceID instanceID, MeshID meshID, const ResourceView<Context>& ctx);

        void destroyMesh(MeshID meshID, const ResourceView<Context>& ctx);

        void destroyInstance(InstanceID instanceID);

        inline const Srstl::KeyVector<UID, MeshHandle>& getMeshHandles() const {
            return m_MeshHandles;
        }

    private:
        UP<ShaderStore>& m_ShaderStore;

        ResourceView<VertexLayout> m_VertexLayout;
        Srstl::KeyVector<UID, MeshHandle> m_MeshHandles;
        std::unordered_map<InstanceID, MeshID> m_InstanceToMeshID;

    };


}