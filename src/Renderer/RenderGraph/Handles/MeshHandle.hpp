#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>
#include <SyriusEngine/Utils/EngineLimits.hpp>
#include <SyriusUtils/Srstl/KeyVector.hpp>
#include <SyriusUtils/DevUtils/TestingMacros.hpp>

#include "../RenderGraphDefs.hpp"

namespace Syrius::Renderer {

    class MeshHandle {
    public:
        MeshHandle() = default;

        MeshHandle(MeshID meshID, const ResourceView<Context>& ctx, const Mesh& mesh,
            const ResourceView<ShaderModule>& vertexShader, const ResourceView<VertexLayout>& vertexDesc);

        ~MeshHandle() = default;

        MeshHandle(const MeshHandle&) = delete;

        MeshHandle(MeshHandle&& other) noexcept;

        MeshHandle& operator=(const MeshHandle&) = delete;

        MeshHandle& operator=(MeshHandle&& other) noexcept;

        void createInstance(InstanceID instanceID);

        void setTransformation(InstanceID instanceID, const Transform& transform);

        void removeInstance(InstanceID instanceID);

        void drawMesh(const ResourceView<Context>& ctx) const;

        MeshID getMeshID() const { return m_MeshID; }

        const Srstl::KeyVector<InstanceID, InstanceData>& getInstanceToTransform() const {
            return m_InstanceToTransform;
        }

    private:
        MeshID m_MeshID = 0;

        ResourceView<VertexBuffer> m_VertexBuffer;
        ResourceView<IndexBuffer> m_IndexBuffer;
        ResourceView<VertexArray> m_VertexArray;

        Size m_InstanceCount = 0;
        Srstl::KeyVector<InstanceID, InstanceData> m_InstanceToTransform;

        SR_GET_PRIVATE_MEMBER(MeshHandle, m_InstanceToTransform);
    };

}