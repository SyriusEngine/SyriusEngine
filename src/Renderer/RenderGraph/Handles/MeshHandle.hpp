#pragma once

#include <SyriusEngine/Renderer/RenderPrimitives.hpp>
#include <SyriusEngine/Utils/EngineLimits.hpp>
#include <SyriusUtils/Srstl/KeyVector.hpp>
#include <SyriusUtils/DevUtils/TestingMacros.hpp>

namespace Syrius::Renderer {

    struct TransformData {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 inverseTranspose = glm::mat4(1.0f);
    };

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

        void setTransformation(InstanceID instanceID, const Transform& transform, const ResourceView<Context>& ctx);

        void removeInstance(InstanceID instanceID);

        void drawMesh(const ResourceView<Context>& ctx) const;

        inline MeshID getMeshID() const { return m_MeshID; }

        inline MaterialID getMaterialID() const { return m_MaterialID; }

        inline void setMaterialID(const MaterialID materialID) { m_MaterialID = materialID; }

    private:
        MeshID m_MeshID = 0;
        MaterialID m_MaterialID = 0;

        ResourceView<VertexBuffer> m_VertexBuffer;
        ResourceView<IndexBuffer> m_IndexBuffer;
        ResourceView<VertexArray> m_VertexArray;

        Size m_InstanceCount = 0;
        Srstl::KeyVector<InstanceID, TransformData> m_InstanceToTransform;

        SR_GET_PRIVATE_MEMBER(MeshHandle, m_InstanceToTransform);
    };

}