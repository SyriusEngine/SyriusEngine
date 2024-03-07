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

        void setTransformation(const glm::mat4& transform);

        void draw() const;

    public:
        MaterialID materialID;

        TransformData transformData;

    private:
        ResourceView<Context>& m_Context;

        ResourceView<VertexBuffer> m_VertexBuffer;
        ResourceView<IndexBuffer> m_IndexBuffer;
        ResourceView<VertexArray> m_VertexArray;

    };

}