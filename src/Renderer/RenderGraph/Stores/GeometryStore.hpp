#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>

#include "../IRenderGraphData.hpp"
#include "../Handles/MeshHandle.hpp"

namespace Syrius::Renderer {

    class GeometryStore: public IRenderGraphData {
    public:
        explicit GeometryStore(const ResourceView<Context>& ctx, RenderGraphContainer* container, const SP<DispatcherManager>& dispatcherManager);

        ~GeometryStore() override = default;

        inline Srstl::KeyVector<MeshID, MeshHandle>& getMeshHandles() {
            return m_MeshHandles;
        }

    private:
        void createMesh(MeshID meshID, const SP<Mesh>& mesh);

        void createInstance(InstanceID instanceID, const SP<MeshID>& meshID);

        void setInstanceTransformation(InstanceID instanceID, const SP<Transform>& transform);

        void destroyMesh(MeshID meshID);

        void destroyInstance(InstanceID instanceID);

    private:
        ResourceView<VertexLayout> m_VertexLayout;
        Srstl::KeyVector<MeshID, MeshHandle> m_MeshHandles;
        std::unordered_map<InstanceID, MeshID> m_InstanceToMeshID;

    };


}