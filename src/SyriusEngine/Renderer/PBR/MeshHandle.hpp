#pragma once

#include "../RenderGraphInclude.hpp"

namespace Syrius{

    struct TransformData{
        glm::mat4 transform         = glm::mat4(1.0f);
        glm::mat4 inverseTranspose  = glm::mat4(1.0f);
    };

    class MeshHandle{
    public:
        MeshHandle(ResourceView<Context>& context,
                   const MeshDesc& mesh,
                   ResourceView<ShaderModule>& vertexShader,
                   ResourceView<VertexLayout>& vertexDesc);

        MeshHandle(const MeshHandle& other) = delete;

        MeshHandle(MeshHandle&& other) noexcept;

        MeshHandle& operator=(const MeshHandle& other) = delete;

        MeshHandle& operator=(MeshHandle&& other) noexcept;

        ~MeshHandle();

        MeshID createInstance();

        void setTransformation(MeshID meshID, const glm::mat4& transform);

        void removeInstance(MeshID meshID);

        void draw() const;

        [[nodiscard]] inline uint64 getInstanceCount() const { return transformData.size(); }

    public:
        MaterialID materialID;

        std::vector<TransformData> transformData;

    private:
        ResourceView<Context>& m_Context;

        ResourceView<VertexBuffer> m_VertexBuffer;
        ResourceView<IndexBuffer> m_IndexBuffer;
        ResourceView<VertexArray> m_VertexArray;

        std::unordered_map<MeshID, uint32> m_InstanceToIndex;

    };

}